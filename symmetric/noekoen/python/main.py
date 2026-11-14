#!/usr/bin/env python3
"""CLI для NOEKEON: CBC + PKCS#7, ключ через SHA-256, вывод в base64."""

import base64
import hashlib
import os
import sys

from noekeon import encrypt_block, decrypt_block

BLOCK = 16

# ---------------------------------------------------------------- padding

def pkcs7_pad(data: bytes) -> bytes:
    n = BLOCK - (len(data) % BLOCK)
    return data + bytes([n]) * n


def pkcs7_unpad(data: bytes) -> bytes:
    if not data or len(data) % BLOCK != 0:
        raise ValueError("invalid padded data length")
    n = data[-1]
    if n < 1 or n > BLOCK:
        raise ValueError("invalid padding byte")
    if data[-n:] != bytes([n]) * n:
        raise ValueError("invalid padding")
    return data[:-n]


# ---------------------------------------------------------------- CBC

def cbc_encrypt(data: bytes, key: bytes, indirect: bool) -> bytes:
    iv = os.urandom(BLOCK)
    padded = pkcs7_pad(data)
    out = bytearray(iv)
    prev = iv
    for i in range(0, len(padded), BLOCK):
        block = bytes(a ^ b for a, b in zip(padded[i:i+BLOCK], prev))
        ct = encrypt_block(block, key, indirect=indirect)
        out.extend(ct)
        prev = ct
    return bytes(out)


def cbc_decrypt(data: bytes, key: bytes, indirect: bool) -> bytes:
    if len(data) < BLOCK or (len(data) - BLOCK) % BLOCK != 0:
        raise ValueError("invalid ciphertext length")
    iv = data[:BLOCK]
    ct = data[BLOCK:]
    prev = iv
    out = bytearray()
    for i in range(0, len(ct), BLOCK):
        block = ct[i:i+BLOCK]
        pt = decrypt_block(block, key, indirect=indirect)
        out.extend(bytes(a ^ b for a, b in zip(pt, prev)))
        prev = block
    return pkcs7_unpad(bytes(out))


# ---------------------------------------------------------------- CLI helpers

def ask(prompt: str) -> str:
    return input(prompt).strip()


def ask_choice(prompt: str, mapping: dict, display: str) -> str:
    """mapping: {ввод: каноническое_значение}. Принимает слово или цифру."""
    while True:
        val = ask(f"{prompt} {display}: ").lower()
        if val in mapping:
            return mapping[val]
        print(f"  Ожидается: {display}")


def main():
    print("=== NOEKEON CBC/base64 ===\n")

    # 1. входной файл
    path_in = ask("Путь к входному файлу: ")
    if not os.path.isfile(path_in):
        print(f"Файл не найден: {path_in}")
        sys.exit(1)

    # 2. действие: слово или 1/2
    action = ask_choice(
        "Действие",
        {
            "encrypt": "encrypt", "e": "encrypt", "1": "encrypt",
            "decrypt": "decrypt", "d": "decrypt", "2": "decrypt",
        },
        "encrypt(1)/decrypt(2)",
    )

    # 3. режим: слово или 1/2
    mode = ask_choice(
        "Режим ключа",
        {
            "direct": "direct", "d": "direct", "1": "direct",
            "indirect": "indirect", "i": "indirect", "2": "indirect",
        },
        "direct(1)/indirect(2)",
    )
    indirect = (mode == "indirect")

    # 4. пароль
    passphrase = ask("Пароль (ключ): ")
    if not passphrase:
        print("Пароль не может быть пустым")
        sys.exit(1)

    # ключ: SHA-256 -> первые 16 байт
    key = hashlib.sha256(passphrase.encode("utf-8")).digest()[:BLOCK]

    # 5. выходной файл: по умолчанию — тот же, что входной (перезапись)
    path_out = ask(f"Путь к выходному файлу (Enter = перезаписать {path_in}): ")
    if not path_out:
        path_out = path_in

    # 6. читаем входной файл целиком в память (важно до записи!)
    raw = open(path_in, "rb").read()

    try:
        if action == "encrypt":
            result = cbc_encrypt(raw, key, indirect)
            text = base64.b64encode(result).decode("ascii")
            with open(path_out, "w", encoding="ascii") as f:
                f.write(text)
        else:
            text = raw.decode("ascii").strip()
            data = base64.b64decode(text)
            result = cbc_decrypt(data, key, indirect)
            with open(path_out, "wb") as f:
                f.write(result)
    except Exception as e:
        print(f"Ошибка: {e}")
        sys.exit(1)

    if path_out == path_in:
        print(f"Готово: {path_out} (файл перезаписан)")
    else:
        print(f"Готово: {path_out}")


if __name__ == "__main__":
    main()