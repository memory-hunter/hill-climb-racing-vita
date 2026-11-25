import argparse
import struct
from pathlib import Path
from zipfile import ZipFile


def check_package(path: Path, title_id: str = "HCR000000") -> None:
    with ZipFile(path) as package:
        data = package.read("sce_sys/param.sfo")
    magic, version, keys, values, count = struct.unpack_from("<5I", data)
    if magic != 0x46535000:
        raise ValueError("Invalid SFO header")
    fields = {}
    for i in range(count):
        key_offset, kind, length, capacity, value_offset = struct.unpack_from("<HHIII", data, 20 + 16 * i)
        key = data[keys + key_offset:].split(b"\0", 1)[0].decode()
        fields[key] = data[values + value_offset:values + value_offset + length].rstrip(b"\0")
    actual_title = fields["TITLE_ID"].decode()
    content = fields["CONTENT_ID"].decode()
    if actual_title != title_id or content[7:16] != title_id:
        raise ValueError(f"Unsafe package identity: TITLE_ID={actual_title}, CONTENT_ID={content}, expected={title_id}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("package", type=Path)
    args = parser.parse_args()
    check_package(args.package)
    print("Package identity verified")
