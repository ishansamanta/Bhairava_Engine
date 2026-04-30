#  Bhairava Engine
### A lightweight cryptographic engine built in C using OpenSSL

---
#  Overview

Bhairava Engine is a secure, lightweight cryptographic system designed for:

 File encryption & decryption  
 Data integrity protection (HMAC)  
 Password-based encryption  
 Easy integration into C projects  
It is built as a reusable security engine, not just a CLI tool.

---

#  Visual Concept
Secure data flow: Input → Encryption Engine → Protected Output


---

#  Quick Start

##  1. Clone the repository 2. Build 3.Encrypt 4.Decrypt

```bash
git clone https://github.com/ishansamanta/Bhairava-Engine.git
cd Bhairava-Engine
make
./bhairav encrypt input.txt output.bhv yourpassword
./bhairav decrypt output.bhv result.txt yourpassword

```
## Integration (For Developers)

Use Bhairava Engine as a library inside your C project:

#include "bhairava_engine.h"

unsigned char output[4096];

int len = bhairav_encrypt_data_secure(
    input_data,
    input_len,
    password,
    output
);

## Security Model

Bhairava Engine uses:

AES-256-CBC encryption
PBKDF2 key derivation (password → secure key)
HMAC-SHA256 integrity check
Random salt + IV per encryption

 Ensures confidentiality
 Ensures integrity
 Prevents tampering

## Example Workflow
```bash
echo "Secret Message" > input.txt
./bhairav encrypt input.txt secure.bhv pass123
./bhairav decrypt secure.bhv output.txt pass123
```
## Real-World Use Cases
1.Secure login systems
2.Encrypted file storage
3.Embedded system security modules
4.IoT device data protection
5.Custom security frameworks

# Glimpse of the Engine
<img width="1600" height="806" alt="2026-04-30" src="https://github.com/user-attachments/assets/ff76009d-b88c-4697-b161-45005bb85bcb" />

1. Remember the password you entered for encrypting the file, which will be later required for decrypting it too. Wrong passwords wont allow you to decrypt it.

2. Maintain the structure and syntax of the command for proper workflow and usage of the engine.

3. More security related updates are coming soon.

## Security Notice
This project is designed for educational and engineering purposes.
While it uses strong cryptographic primitives, it is not yet formally audited for enterprise-grade production security.
Contributors are welcomed.
------------------------------------------------------------------------------------------------------------------
Author
Ishan Samanta
BTech CSE (IoT)
Made by ISHAN ORGANISATION

_________License________

This project is licensed under the MIT License.
MIT License
Permission is hereby granted, free of charge, to use, copy, modify, and distribute this software.


“Security is not a feature. It is a foundation.”
Bhairava Engine is built as a step toward understanding real-world cryptographic systems and secure software design.
