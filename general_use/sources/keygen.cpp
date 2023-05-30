#include "keygen.h"

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <variables.h>

#include <QDebug>
#include <QRandomGenerator>
#include <iostream>

#define PUB_EXP 65537

void seedRandom() {
    QRandomGenerator prng(time(nullptr));

    uint8_t seed_data[256];
    for (size_t i = 0; i < sizeof(seed_data) / 4; i += 4) {
        uint32_t random_number = prng.generate();
        for (size_t j = 0; j < 4; ++j) {
            seed_data[i + j] = (random_number << (8 * j));
        }
    }

    RAND_seed(seed_data, sizeof(seed_data));
}

QPair<QSslKey, QSslCertificate> generateKeyPair() {
    seedRandom();
    EVP_PKEY* pkey = nullptr;
    RSA* rsa = nullptr;
    X509* x509 = nullptr;
    X509_NAME* name = nullptr;
    BIO *bp_public = nullptr, *bp_private = nullptr;
    const char* buffer = nullptr;
    long size;
    int ret;

    pkey = EVP_PKEY_new();
    q_check_ptr(pkey);
    BIGNUM* bne = NULL;
    bne = BN_new();
    ret = BN_set_word(bne, PUB_EXP);
    if (ret != 1) {
        qFatal("OPENSSL: Unable to generate BIGNUM");
    }
    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, 2048, bne, NULL);
    if (ret != 1) {
        qFatal("OPENSSL: Unable to generate RSA key");
    }
    q_check_ptr(rsa);
    EVP_PKEY_assign_RSA(pkey, rsa);
    x509 = X509_new();
    q_check_ptr(x509);
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);         // not before current time
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);  // not after a year from this point
    X509_set_pubkey(x509, pkey);
    name = X509_get_subject_name(x509);
    q_check_ptr(name);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"My Organization", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"My Common Name", -1, -1, 0);
    X509_set_issuer_name(x509, name);
    X509_sign(x509, pkey, EVP_sha1());
    bp_private = BIO_new(BIO_s_mem());
    q_check_ptr(bp_private);
    if (PEM_write_bio_PrivateKey(bp_private, pkey, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        EVP_PKEY_free(pkey);
        X509_free(x509);
        BIO_free_all(bp_private);
        qFatal("OPENSSL: PEM_write_bio_PrivateKey");
    }
    bp_public = BIO_new(BIO_s_mem());
    q_check_ptr(bp_public);
    if (PEM_write_bio_X509(bp_public, x509) != 1) {
        EVP_PKEY_free(pkey);
        X509_free(x509);
        BIO_free_all(bp_public);
        BIO_free_all(bp_private);
        qFatal("OPENSSL: PEM_write_bio_PrivateKey");
    }
    size = BIO_get_mem_data(bp_public, &buffer);
    q_check_ptr(buffer);
    QSslCertificate cert = QSslCertificate(QByteArray(buffer, size));
    if (cert.isNull()) {
        qFatal("OPENSSL: Failed to generate a random client certificate");
    }
    size = BIO_get_mem_data(bp_private, &buffer);
    q_check_ptr(buffer);
    QSslKey key = QSslKey(QByteArray(buffer, size), QSsl::Rsa);
    if (key.isNull()) {
        qFatal("OPENSSL: Failed to generate a random private key");
    }

    EVP_PKEY_free(pkey);  // this will also free the rsa key
    X509_free(x509);
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    return {key, cert};
}

QByteArray generate_AES_key_128() {
    seedRandom();
    uint8_t key[EVP_MAX_KEY_LENGTH];
    constexpr uint16_t num_bytes = 128 / 8;
    RAND_bytes(key, num_bytes);
    return QByteArray(reinterpret_cast<const char*>(key), num_bytes);
}
