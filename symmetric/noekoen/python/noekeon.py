"""NOEKEON block cipher (128-bit block, 128-bit key, 16 rounds)."""

import struct

MASK32 = 0xFFFFFFFF

# ---------------------------------------------------------------- утилиты

def _rotl(x: int, n: int) -> int:
    n &= 31
    return ((x << n) | (x >> (32 - n))) & MASK32

def _rotr(x: int, n: int) -> int:
    n &= 31
    return ((x >> n) | (x << (32 - n))) & MASK32

def _bytes_to_words(block: bytes):
    if len(block) != 16:
        raise ValueError("block must be exactly 16 bytes")
    return list(struct.unpack(">4I", block))

def _words_to_bytes(words) -> bytes:
    return struct.pack(">4I", *[w & MASK32 for w in words])

# ---------------------------------------------------------------- преобразования

def _theta(key_words, state, inverse=False):
    """Theta: добавление ключа + линейная диффузия. Инволюция."""
    a0, a1, a2, a3 = state
    k0, k1, k2, k3 = key_words

    if not inverse:
        # 1. нечётные слова
        t = a0 ^ a2
        t ^= _rotl(t, 8) ^ _rotr(t, 8)
        a1 ^= t
        a3 ^= t
        # 2. ключ
        a0 ^= k0; a1 ^= k1; a2 ^= k2; a3 ^= k3
        # 3. чётные слова
        t = a1 ^ a3
        t ^= _rotl(t, 8) ^ _rotr(t, 8)
        a0 ^= t
        a2 ^= t
    else:
        # обратный порядок (инволюция, но с ключом посередине)
        t = a1 ^ a3
        t ^= _rotl(t, 8) ^ _rotr(t, 8)
        a0 ^= t
        a2 ^= t

        a0 ^= k0; a1 ^= k1; a2 ^= k2; a3 ^= k3

        t = a0 ^ a2
        t ^= _rotl(t, 8) ^ _rotr(t, 8)
        a1 ^= t
        a3 ^= t

    state[:] = [a0 & MASK32, a1 & MASK32, a2 & MASK32, a3 & MASK32]

def _pi1(state):
    state[1] = _rotl(state[1], 1)
    state[2] = _rotl(state[2], 5)
    state[3] = _rotl(state[3], 2)

def _pi1_inv(state):
    state[1] = _rotr(state[1], 1)
    state[2] = _rotr(state[2], 5)
    state[3] = _rotr(state[3], 2)

def _pi2(state):
    state[1] = _rotr(state[1], 1)
    state[2] = _rotr(state[2], 5)
    state[3] = _rotr(state[3], 2)

def _pi2_inv(state):
    state[1] = _rotl(state[1], 1)
    state[2] = _rotl(state[2], 5)
    state[3] = _rotl(state[3], 2)

def _gamma(state):
    """Нелинейное преобразование Gamma (инволюция)."""
    a0, a1, a2, a3 = state
    a1 ^= (~(a3 | a2)) & MASK32
    a0 ^= a2 & a1
    a3, a0 = a0, a3          # swap
    a2 ^= a0 ^ a1 ^ a3
    a1 ^= (~(a3 | a2)) & MASK32
    a0 ^= a2 & a1
    state[:] = [a0 & MASK32, a1 & MASK32, a2 & MASK32, a3 & MASK32]

# ---------------------------------------------------------------- константы раундов

_ROUND_CONSTANTS = (
    0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
    0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
    0xD4, 0xB3, 0x7D, 0xFA, 0xEF, 0xC5, 0x91, 0x39,
    0x72, 0xE4, 0xD3, 0xBD, 0x61, 0xC2, 0x9F, 0x25,
    0x4A, 0x94,
)

# ---------------------------------------------------------------- раунды

def _round(key_words, state, constant):
    state[0] ^= constant
    _theta(key_words, state)
    _pi1(state)
    _gamma(state)
    _pi2(state)

def _round_inv(key_words, state, constant):
    _pi2_inv(state)
    _gamma(state)
    _pi1_inv(state)
    _theta(key_words, state, inverse=True)
    state[0] ^= constant

def _encrypt_rounds(state, working_key_words):
    for i in range(16):
        _round(working_key_words, state, _ROUND_CONSTANTS[i])
    state[0] ^= _ROUND_CONSTANTS[16]
    _theta(working_key_words, state)

def _decrypt_rounds(state, working_key_words):
    _theta(working_key_words, state, inverse=True)
    state[0] ^= _ROUND_CONSTANTS[16]
    for i in range(15, -1, -1):
        _round_inv(working_key_words, state, _ROUND_CONSTANTS[i])

# ---------------------------------------------------------------- публичный API

def _derive_working_key(key: bytes) -> bytes:
    zero_key = b"\x00" * 16
    return encrypt_block(key, zero_key, indirect=False)

def encrypt_block(block: bytes, key: bytes, indirect: bool = False) -> bytes:
    if len(block) != 16 or len(key) != 16:
        raise ValueError("block and key must be 16 bytes each")
    working_key = _derive_working_key(key) if indirect else key
    state = _bytes_to_words(block)
    key_words = _bytes_to_words(working_key)
    _encrypt_rounds(state, key_words)
    return _words_to_bytes(state)

def decrypt_block(block: bytes, key: bytes, indirect: bool = False) -> bytes:
    if len(block) != 16 or len(key) != 16:
        raise ValueError("block and key must be 16 bytes each")
    working_key = _derive_working_key(key) if indirect else key
    state = _bytes_to_words(block)
    key_words = _bytes_to_words(working_key)
    _decrypt_rounds(state, key_words)
    return _words_to_bytes(state)

# ---------------------------------------------------------------- самопроверка

if __name__ == "__main__":
    # --- Round-trip ---
    pt = bytes(16)
    key = bytes(16)
    ct = encrypt_block(pt, key)
    pt2 = decrypt_block(ct, key)
    print("round-trip:", "OK" if pt2 == pt else "FAIL")

    # --- Тестовый вектор из спецификации NESSIE (direct mode) ---
    # plaintext  = 4765F3DA 10CD3D04 73867742 B5E5CC3C
    # key        = 2BD6459F 82C5B300 952C4910 4881FF48
    # ciphertext = EA024714 AD5C4D84 EA024714 AD5C4D84
    pt_hex = "4765F3DA10CD3D0473867742B5E5CC3C"
    key_hex = "2BD6459F82C5B300952C49104881FF48"
    ct_expected_hex = "EA024714AD5C4D84EA024714AD5C4D84"

    pt_bytes = bytes.fromhex(pt_hex)
    key_bytes = bytes.fromhex(key_hex)
    ct_bytes = encrypt_block(pt_bytes, key_bytes, indirect=False)

    print("test vector (direct):", "OK" if ct_bytes.hex().upper() == ct_expected_hex else "FAIL")
    if ct_bytes.hex().upper() != ct_expected_hex:
        print("  got     :", ct_bytes.hex().upper())
        print("  expected:", ct_expected_hex)