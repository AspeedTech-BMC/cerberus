"""
Copyright (c) Microsoft Corporation. All rights reserved.
Licensed under the MIT license.
"""

from __future__ import print_function
from __future__ import unicode_literals
import os
import sys
import base64
import traceback
import xml.etree.ElementTree as et
import binascii
import re
import ctypes
from Crypto.PublicKey import RSA
from Crypto.Signature import PKCS1_v1_5
from Crypto.Hash import SHA256

IMAGE_CONFIG_FILENAME = "decommission_image_generator.config"

KEY_CANCELLATION_IMAGE_TYPE = int(4)
DECOMMISSION_IMAGE_TYPE = int(5)

XML_VERSION_ATTRIB = "version"
XML_PLATFORM_ATTRIB = "platform"
XML_TYPE_ATTRIB = "type"

XML_REGION_TAG = "Region"
XML_WRITE_ADDRESS_TAG = "WriteAddress"
XML_ENCODED_IMAGE_TAG = "EncodedImage"
XML_START_ADDR_TAG = "StartAddr"
XML_END_ADDR_TAG = "EndAddr"

XML_CANCELLATION_SECTION_TAG = "CancellationSection"
XML_DECOMMISSION_IMAGE_TAG = "DecommissionImage"

IMAGE_MAGIC_NUM = int("0xb6eafd19", 16)
IMAGE_SECTION_MAGIC_NUM = int("0xf27f28d7", 16)
IMAGE_SECTION_FORMAT_NUM = 0
KEY_CANCELLATION_IMAGE_SECTION_HEADER_LENGTH = 18
DECOMMISSION_IMAGE_SECTION_HEADER_LENGTH = 0
IMAGE_MAX_SIZE = 134217728
IMAGE_MAX_VERSION_ID_SIZE = 32

# BMC PFM CANCELLATION
CANCELLATION_IMAGE_KEY_TYPE = int("0x0103", 16)


class image_header(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [('header_length', ctypes.c_ushort),
                ('type', ctypes.c_ushort),
                ('marker', ctypes.c_uint),
                ('image_length', ctypes.c_uint),
                ('sig_length', ctypes.c_uint),
                ('platform_id_length', ctypes.c_ubyte),
                ('pubkey_length', ctypes.c_uint)]


class image_section_header(ctypes.LittleEndianStructure):
    _pack_ = 1
    _fields_ = [('header_length', ctypes.c_ushort),
                ('format', ctypes.c_ushort),
                ('marker', ctypes.c_uint),
                ('addr', ctypes.c_uint),
                ('image_length', ctypes.c_uint),
                ('key_type', ctypes.c_ushort)]


def process_image(root):
    """
    Process the tree storing the image data starting with the root element

    :param root: The root element for the tree storing the XML image data

    :return dictionary of the processed image data
    """

    xml = {}

    version_id = root.attrib.get(XML_VERSION_ATTRIB)
    type_xml = root.attrib.get(XML_TYPE_ATTRIB)

    if (version_id in (None, "") or (len(version_id) > (IMAGE_MAX_VERSION_ID_SIZE - 1))):
        raise ValueError("Invalid or no image version ID provided")

    platform_id = root.attrib.get(XML_PLATFORM_ATTRIB)

    if platform_id in (None, ""):
        raise ValueError("No Platform ID provided")

    xml["version_id"] = version_id.strip().encode("utf8")
    xml["platform_id"] = platform_id.strip().encode("utf8")
    xml["type"] = int(type_xml)

    if xml["type"] != KEY_CANCELLATION_IMAGE_TYPE and xml["type"] != DECOMMISSION_IMAGE_TYPE:
        raise ValueError("Invalid number of type in the image: {0}".format(xml["type"]))

    if xml["type"] == KEY_CANCELLATION_IMAGE_TYPE:
        sections = root.findall(XML_CANCELLATION_SECTION_TAG)

        if not sections:
            raise ValueError("Invalid number of Sections tags in the image: {0}".format(xml["version_id"]))

        xml["sections"] = []
        with open(input_bin_path, "rb") as f:
            imagedata = f.read()
        for s in sections:
            sectiondata = b""
            image_section = {}
            write_addr = s.findall(XML_WRITE_ADDRESS_TAG)

            if not write_addr or len(write_addr) > 1:
                raise ValueError("Invalid number of WriteAddress tags in the image: {0}".format(
                    xml["version_id"]))

            image_section["addr"] = write_addr[0].text.strip()
            regions = s.findall(XML_REGION_TAG)
            for region in regions:
                startaddress = region.findall(XML_START_ADDR_TAG)[0].text.strip()
                endaddress = region.findall(XML_END_ADDR_TAG)[0].text.strip()
                sectiondata = sectiondata + imagedata[int(startaddress, 16):int(endaddress, 16) + 1]

            image_section["image"] = sectiondata
            xml["sections"].append(image_section)

        if not xml["sections"]:
            raise ValueError("No sections found for image: {0}".format(xml["version_id"]))

    return xml


def load_config(config_file):
    """
    Load configuration options from file

    :param config_file: Path for a text file containing config options

    :return parsed configuration
    """

    config = {}
    config["xml"] = ""
    config["output"] = ""
    config["input"] = ""
    config["prv_key_path"] = ""
    config["key_size"] = ""
    config["cancel_key"] = ""

    with open(config_file, 'r') as fh:
        data = fh.readlines()

    if not data:
        print("Failed to load configuration")
        sys.exit(1)

    for string in data:
        string = string.replace("\n", "")
        string = string.replace("\r", "")

        if string.startswith("Output"):
            config["output"] = string.split("=")[-1].strip()
        elif string.startswith("InputImage"):
            config["input"] = string.split("=")[-1].strip()
        elif string.startswith("KeySize"):
            config["key_size"] = string.split("=")[-1].strip()
        elif string.startswith("Key"):
            config["prv_key_path"] = string.split("=")[-1].strip()
        elif string.startswith("Xml"):
            config["xml"] = string.split("=")[-1].strip()
        elif string.startswith("Cancel_Key"):
            config["cancel_key"] = string.split("=")[-1].strip()

    return config


def load_and_process_xml(xml_file):
    """
    Process the XML file storing the image data

    :param xml_file: Name of XML file storing the image data

    :return dictionary of the processed image data
    """

    root = et.parse(xml_file).getroot()
    return process_image(root)


def get_image_len(xml, sig_len):
    """
    Calculate the image length from the processed image data. The total includes
    the headers, image(s), and signature.

    :param xml: The processed image data
    :param sig_len: The image signature length

    :return the total length of the image
    """

    header_len = 49 + len(xml["platform_id"])

    image_lens = 0
    if xml["type"] == KEY_CANCELLATION_IMAGE_TYPE:
        for section in xml["sections"]:
            image_lens += len(section["image"]) + KEY_CANCELLATION_IMAGE_SECTION_HEADER_LENGTH

    return header_len + image_lens + sig_len


def generate_image_section_instance(section):
    """
    Create a image section

    :param section: The image section data

    :return instance of a image section
    """

    addr = int(section["addr"], 16)
    section_header = image_section_header(KEY_CANCELLATION_IMAGE_SECTION_HEADER_LENGTH,
                                          IMAGE_SECTION_FORMAT_NUM,
                                          IMAGE_SECTION_MAGIC_NUM,
                                          addr,
                                          len(section["image"]),
                                          CANCELLATION_IMAGE_KEY_TYPE)

    img_array = (ctypes.c_ubyte * len(section["image"])).from_buffer_copy(section["image"])

    class _image_section(ctypes.LittleEndianStructure):
        _pack_ = 1
        _fields_ = [('header', image_section_header),
                    ('img', ctypes.c_ubyte * len(section["image"]))]

    return _image_section(section_header, img_array)


def generate_image_section_list(xml):
    """
    Create a list of image sections from the parsed XML list

    :param xml: List of parsed XML image data

    :return list of image section instances
    """

    section_list = []
    min_addr = -1

    for s in xml["sections"]:
        sec_addr = int(s["addr"], 16)
        if sec_addr <= min_addr:
            raise ValueError("Invalid WriteAddress in image section: {0}".format(
                s["addr"]))
        section = generate_image_section_instance(s)
        section_list.append(section)
        min_addr = sec_addr + len(s["image"]) - 1

    return section_list


def generate_image(xml, image_header_instance, image_sections_list):
    """
    Create a image object from all the different image components

    :param xml: List of parsed XML image data
    :param image_header_instance: Instance of a image header
    :param image_sections_list: List of image sections to be included in the image

    :return Instance of a image object
    """

    sections_size = 0

    for section in image_sections_list:
        sections_size += section.header.header_length + section.header.image_length

    version_len = len(xml["version_id"])
    xml["version_id"] = xml["version_id"].decode() + ''.join('\x00' for i in range(version_len, 32))
    version_id_str_buf = ctypes.create_string_buffer(xml["version_id"].encode('utf-8'), 32)
    version_id_buf = (ctypes.c_ubyte * 32)()
    ctypes.memmove(ctypes.addressof(version_id_buf), ctypes.addressof(version_id_str_buf), 32)

    xml["platform_id"] = xml["platform_id"].decode() + '\x00'
    platform_id_str_buf = ctypes.create_string_buffer(xml["platform_id"].encode('utf-8'), len(xml["platform_id"]))
    platform_id_buf = (ctypes.c_ubyte * len(xml["platform_id"]))()
    ctypes.memmove(ctypes.addressof(platform_id_buf),
                   ctypes.addressof(platform_id_str_buf),
                   len(xml["platform_id"]))

    class complete_header(ctypes.LittleEndianStructure):
        _pack_ = 1
        _fields_ = [('header_length', ctypes.c_ushort),
                    ('format', ctypes.c_ushort),
                    ('marker', ctypes.c_uint),
                    ('version_id', ctypes.c_ubyte * 32),
                    ('image_length', ctypes.c_uint),
                    ('sig_length', ctypes.c_uint),
                    ('platform_id_length', ctypes.c_ubyte),
                    ('platform_id', ctypes.c_ubyte * len(xml["platform_id"]))]

    class generated_image(ctypes.LittleEndianStructure):
        _pack_ = 1
        _fields_ = [('image_header', ctypes.c_ubyte *
                     image_header_instance.header_length),
                    ('image_sections', ctypes.c_ubyte * sections_size)]

    complete_header_instance = complete_header(image_header_instance.header_length,
                                               image_header_instance.type,
                                               image_header_instance.marker,
                                               version_id_buf,
                                               image_header_instance.image_length,
                                               image_header_instance.sig_length,
                                               image_header_instance.platform_id_length,
                                               platform_id_buf)

    header_buf = (ctypes.c_ubyte * image_header_instance.header_length)()
    ctypes.memmove(ctypes.addressof(header_buf),
                   ctypes.addressof(complete_header_instance),
                   image_header_instance.header_length)

    offset = 0
    sections_buf = (ctypes.c_ubyte * sections_size)()

    for section in image_sections_list:
        ctypes.memmove(ctypes.addressof(sections_buf) + offset,
                       ctypes.addressof(section),
                       section.header.header_length + section.header.image_length)
        offset += section.header.header_length + section.header.image_length

    return generated_image(header_buf, sections_buf)


def write_final_image(sign, generated_image, key, output_file_name, image_header):
    """
    Write image generated to provided path.

    :param sign: Boolean indicating whether to sign the image or not
    :param generated_image: Generated image to write
    :param key: Key to use for signing
    :param output_filename: Name to use for output file
    :param image_header: The image header instance

    """
    image_length = image_header.image_length - image_header.sig_length
    if ctypes.sizeof(generated_image) > (IMAGE_MAX_SIZE - image_header.sig_length):
        raise ValueError("Generated image is too large - {0}".format(ctypes.sizeof(generated_image)))

    if sign:
        hash_buf = (ctypes.c_ubyte * ctypes.sizeof(generated_image))()
        ctypes.memmove(ctypes.addressof(hash_buf), ctypes.addressof(generated_image), ctypes.sizeof(generated_image))
        h = SHA256.new(hash_buf)
        signer = PKCS1_v1_5.new(key)
        signature = signer.sign(h)
        signature = (ctypes.c_ubyte * image_header.sig_length).from_buffer_copy(signature)
        image_buf = (ctypes.c_char * (image_header.image_length))()
        ctypes.memmove(ctypes.byref(image_buf, image_length),
                       ctypes.addressof(signature),
                       image_header.sig_length)
    else:
        image_buf = (ctypes.c_char * (image_length))()
    with open(output_file_name, 'wb') as fh:
        ctypes.memmove(ctypes.byref(image_buf), ctypes.addressof(generated_image), image_length)
        fh.write(image_buf)
        fh.write(PubKey)


def load_key(key_size, prv_key_path):
    """
    Load private RSA key to sign the image from the provided path. If no valid key can be
    imported, key size will be what is provided. Otherwise, key size will be size of key imported.

    :param key_size: Provided key_size
    :param prv_key_path: Provided private key path

    :return <Sign image or not> <key_size> <Key to use for signing>
    """

    if prv_key_path:
        try:
            key = RSA.importKey(open(prv_key_path).read())
        except Exception:
            print("Unsigned image will be generated, provided RSA key could not be imported: {0}".format(prv_key_path))
            traceback.print_exc()
            return False, key_size, None

        return True, int(key.n.bit_length() / 8), key
    else:
        print("No RSA private key provided in config, unsigned image will be generated.")
        return False, key_size, None


def generate_cancel_public_key(cancel_key_path, input_bin_path):
    retval = True

    try:
        key_load, key_size, key = load_key(None, cancel_key_path)
        if key_load:
            mod_fmt = "%%0%dx" % (key.n.bit_length() // 4)
            modulus = binascii.a2b_hex(mod_fmt % key.n)
            with open(input_bin_path, 'wb+') as fh:
                fh.write(modulus)
        else:
            print("Failed to load the cancel key..")
            retval = False
    except Exception as err:
        print("Error in creating the cancel public key: ", err)
        retval = False
    return retval

# *************************************** Start of Script ***************************************


if len(sys.argv) < 2:
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), IMAGE_CONFIG_FILENAME)
else:
    path = os.path.abspath(sys.argv[1])

config = load_config(path)
key_size = None
prv_key_path = None
input_bin_path = None
cancel_key_path = None

if "key_size" in config and config["key_size"]:
    key_size = int(config["key_size"])

if "prv_key_path" in config and config["prv_key_path"]:
    prv_key_path = config["prv_key_path"]

if "input" in config and config["input"]:
    input_bin_path = config["input"]

if "cancel_key" in config and config["cancel_key"]:
    cancel_key_path = config["cancel_key"]
    generate_cancel_public_key(cancel_key_path, input_bin_path)

sign, key_size, key = load_key(key_size, prv_key_path)

if sign is True:
    mod_fmt = "%%0%dx" % (key.n.bit_length() // 4)
    modulus = binascii.a2b_hex(mod_fmt % key.n)
    exponent = hex(key.e)[2:]
    while (len(exponent) % 2 != 0):
        exponent = '0' + exponent
    exponent = bytearray.fromhex(exponent)
    m_length = hex(len(modulus))[2:]
    while (len(m_length) != 4):
        m_length = "0" + m_length
    e_length = hex(len(exponent))[2:]
    while (len(e_length) != 2):
        e_length = "0" + e_length
    PubKey = (bytearray.fromhex(m_length)[::-1] + modulus
              + bytearray.fromhex(e_length) + exponent)

sig_len = 0 if key_size is None else key_size

processed_xml = load_and_process_xml(config["xml"])

platform_id_len = len(processed_xml["platform_id"])
header_len = 49 + platform_id_len
image_len = get_image_len(processed_xml, sig_len)
type_in_xml = processed_xml['type']
image_header_instance = image_header(header_len,
                                     type_in_xml,
                                     IMAGE_MAGIC_NUM,
                                     image_len,
                                     sig_len,
                                     platform_id_len)

image_sections_list = []
if type_in_xml == KEY_CANCELLATION_IMAGE_TYPE:
    image_sections_list = generate_image_section_list(processed_xml)

_image = generate_image(processed_xml, image_header_instance, image_sections_list)
write_final_image(sign, _image, key, config["output"], image_header_instance)

print("Completed image generation: {0}".format(config["output"]))
