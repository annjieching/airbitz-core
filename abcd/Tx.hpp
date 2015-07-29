/*
 *  Copyright (c) 2014, Airbitz
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms are permitted provided that
 *  the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  3. Redistribution or use of modified source code requires the express written
 *  permission of Airbitz Inc.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  The views and conclusions contained in the software and documentation are those
 *  of the authors and should not be interpreted as representing official policies,
 *  either expressed or implied, of the Airbitz Project.
 */
/**
 * @file
 * Functions for creating, viewing, and modifying transaction meta-data.
 */

#ifndef ABC_Tx_h
#define ABC_Tx_h

#include "util/Status.hpp"
#include <map>

namespace abcd {

struct SendInfo;
typedef std::map<const std::string, std::string> KeyTable;
class Wallet;

/**
 * Information about a transaction that has been sent
 * but not yet saved to the database.
 */
typedef struct sABC_UnsavedTx
{
    /** Tx Id we use internally */
    char *szTxId;
    /** block chain tx id**/
    char *szTxMalleableId;
    /** Number for outputs **/
    unsigned int countOutputs;
    /** The output information **/
    tABC_TxOutput **aOutputs;
} tABC_UnsavedTx;

tABC_CC ABC_TxDupDetails(tABC_TxDetails **ppNewDetails,
                         const tABC_TxDetails *pOldDetails,
                         tABC_Error *pError);

void ABC_TxFreeDetails(tABC_TxDetails *pDetails);
void ABC_UnsavedTxFree(tABC_UnsavedTx *pUtx);
void ABC_TxFreeOutputs(tABC_TxOutput **aOutputs, unsigned int count);

/**
 * Calculates the private keys for a wallet.
 */
Status
txKeyTableGet(KeyTable &result, Wallet &self);

/**
 * Gets the next unused change address from the wallet.
 */
Status
txNewChangeAddress(std::string &result, Wallet &self,
    tABC_TxDetails *pDetails);

tABC_CC ABC_TxSendComplete(Wallet &self,
                           SendInfo         *pInfo,
                           tABC_UnsavedTx   *pUtx,
                           tABC_Error       *pError);

tABC_CC ABC_TxBlockHeightUpdate(uint64_t height,
                                tABC_BitCoin_Event_Callback fAsyncBitCoinEventCallback,
                                void *pData,
                                tABC_Error *pError);

tABC_CC ABC_TxReceiveTransaction(Wallet &self,
                                 uint64_t amountSatoshi, uint64_t feeSatoshi,
                                 tABC_TxOutput **paInAddress, unsigned int inAddressCount,
                                 tABC_TxOutput **paOutAddresses, unsigned int outAddressCount,
                                 const char *szTxId, const char *szMalTxId,
                                 tABC_BitCoin_Event_Callback fAsyncBitCoinEventCallback,
                                 void *pData,
                                 tABC_Error *pError);

tABC_CC ABC_TxCreateInitialAddresses(Wallet &self,
                                     tABC_Error *pError);

tABC_CC ABC_TxCreateReceiveRequest(Wallet &self,
                                   tABC_TxDetails *pDetails,
                                   char **pszRequestID,
                                   bool bTransfer,
                                   tABC_Error *pError);

tABC_CC ABC_TxModifyReceiveRequest(Wallet &self,
                                   const char *szRequestID,
                                   tABC_TxDetails *pDetails,
                                   tABC_Error *pError);

tABC_CC ABC_TxFinalizeReceiveRequest(Wallet &self,
                                     const char *szRequestID,
                                     tABC_Error *pError);

tABC_CC ABC_TxCancelReceiveRequest(Wallet &self,
                                   const char *szRequestID,
                                   tABC_Error *pError);

tABC_CC ABC_TxGenerateRequestQRCode(Wallet &self,
                                    const char *szRequestID,
                                    char **pszURI,
                                    unsigned char **paData,
                                    unsigned int *pWidth,
                                    tABC_Error *pError);

tABC_CC ABC_TxGetTransaction(Wallet &self,
                             const char *szID,
                             tABC_TxInfo **ppTransaction,
                             tABC_Error *pError);

tABC_CC ABC_TxGetTransactions(Wallet &self,
                              int64_t startTime,
                              int64_t endTime,
                              tABC_TxInfo ***paTransactions,
                              unsigned int *pCount,
                              tABC_Error *pError);

tABC_CC ABC_TxSearchTransactions(Wallet &self,
                                 const char *szQuery,
                                 tABC_TxInfo ***paTransactions,
                                 unsigned int *pCount,
                                 tABC_Error *pError);

void ABC_TxFreeTransaction(tABC_TxInfo *pTransactions);

void ABC_TxFreeTransactions(tABC_TxInfo **aTransactions,
                            unsigned int count);

tABC_CC ABC_TxSetTransactionDetails(Wallet &self,
                                    const char *szID,
                                    tABC_TxDetails *pDetails,
                                    tABC_Error *pError);

tABC_CC ABC_TxGetTransactionDetails(Wallet &self,
                                    const char *szID,
                                    tABC_TxDetails **ppDetails,
                                    tABC_Error *pError);

tABC_CC ABC_TxGetRequestAddress(Wallet &self,
                                const char *szRequestID,
                                char **pszAddress,
                                tABC_Error *pError);

tABC_CC ABC_TxGetPendingRequests(Wallet &self,
                                 tABC_RequestInfo ***paRequests,
                                 unsigned int *pCount,
                                 tABC_Error *pError);

void ABC_TxFreeRequests(tABC_RequestInfo **aRequests,
                        unsigned int count);

tABC_CC ABC_TxSweepSaveTransaction(Wallet &wallet,
                                   const char *txId,
                                   const char *malTxId,
                                   uint64_t funds,
                                   tABC_TxDetails *pDetails,
                                   tABC_Error *pError);

// Blocking functions:
tABC_CC ABC_TxGetPubAddresses(Wallet &self,
                            char ***paAddresses,
                            unsigned int *pCount,
                            tABC_Error *pError);

tABC_CC ABC_TxWatchAddresses(Wallet &self,
                             tABC_Error *pError);

} // namespace abcd

#endif
