#pragma once

#include <QPair>
#include <QSslCertificate>
#include <QSslKey>

/*!
 * \brief Generates public and private keys.
 */
QPair<QSslKey, QSslCertificate> generateKeyPair();
