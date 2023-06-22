#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <QByteArray>
#include <QPair>
#include <QSslCertificate>
#include <QSslKey>

void seedRandom();
/*!
 * \brief Generates public and private keys.
 */
QPair<QSslKey, QSslCertificate> generateKeyPair();

/*!
 * \brief Generates 128bit AES key.
 */
QByteArray generate_AES_key_128();
