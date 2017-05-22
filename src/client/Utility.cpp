/************************************************************
 *
 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  EMAIL:
 *  fellowtraveler@opentransactions.org
 *
 *  WEBSITE:
 *  http://www.opentransactions.org/
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This Source Code Form is subject to the terms of the
 *   Mozilla Public License, v. 2.0. If a copy of the MPL
 *   was not distributed with this file, You can obtain one
 *   at http://mozilla.org/MPL/2.0/.
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will
 *   be useful, but WITHOUT ANY WARRANTY; without even the
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A
 *   PARTICULAR PURPOSE.  See the Mozilla Public License
 *   for more details.
 *
 ************************************************************/

#include "opentxs/client/Utility.hpp"

#include "opentxs/client/OTAPI_Wrap.hpp"
#include "opentxs/client/OT_ME.hpp"
#include "opentxs/core/Log.hpp"

#include <stdint.h>
#include <ostream>
#include <string>

namespace opentxs
{

using namespace std;

OT_UTILITY_OT bool VerifyMessage(const string& strMessage)
{
    if (10 > strMessage.length()) {
        otWarn << "VerifyMessage: Error strMessage is: Too Short:" << strMessage
               << "\n";
        return false;
    }
    return true;
}

OT_UTILITY_OT int32_t VerifyMessageSuccess(const string& strMessage)
{
    if (!VerifyMessage(strMessage)) {
        return -1;
    }

    int32_t nStatus = OTAPI_Wrap::Message_GetSuccess(strMessage);
    switch (nStatus) {
    case -1:
        otOut << "VerifyMessageSuccess: Error calling "
                 "OT_API_Message_GetSuccess, for message:\n\n" << strMessage
              << "\n";
        break;
    case 0:
        otWarn << "VerifyMessageSuccess: Reply received: success == FALSE. "
                  "Reply message:\n\n" << strMessage << "\n";
        break;
    case 1:
        otWarn << "VerifyMessageSuccess: Reply received: success == TRUE.\n";
        break;
    default:
        otOut << "VerifyMessageSuccess: Error. (This should never happen!) "
                 "nStatus: " << nStatus << ", Input: " << strMessage << "\n";
        nStatus = -1;
        break;
    }

    return nStatus;
}

OT_UTILITY_OT int32_t VerifyMsgBalanceAgrmntSuccess(
    const string& NOTARY_ID, const string& NYM_ID, const string& ACCOUNT_ID,
    const string& strMessage) // For when an OTMessage is the input.
{
    if (!VerifyMessage(strMessage)) {
        return -1;
    }

    int32_t nSuccess = OTAPI_Wrap::Message_GetBalanceAgreementSuccess(
        NOTARY_ID, NYM_ID, ACCOUNT_ID, strMessage);
    switch (nSuccess) {
    case -1:
        otOut << "VerifyMsgBalanceAgrmntSuccess: Error calling "
                 "OT_API_Msg_GetBlnceAgrmntSuccess, for message:\n\n"
              << strMessage << "\n";
        break;
    case 0:
        otWarn << "VerifyMsgBalanceAgrmntSuccess: Reply received: success == "
                  "FALSE. Reply message:\n\n" << strMessage << "\n";
        break;
    case 1:
        otWarn << "VerifyMsgBalanceAgrmntSuccess: Reply received: success == "
                  "TRUE.\n";
        break;
    default:
        otOut << "VerifyMsgBalanceAgrmntSuccess: Error. (This should never "
                 "happen!) Input: " << strMessage << "\n";
        nSuccess = -1;
        break;
    }

    return nSuccess;
}

OT_UTILITY_OT int32_t
    VerifyMsgTrnxSuccess(const string& NOTARY_ID, const string& NYM_ID,
                         const string& ACCOUNT_ID, const string& strMessage)
{
    if (!VerifyMessage(strMessage)) {
        return -1;
    }

    int32_t nSuccess = OTAPI_Wrap::Message_GetTransactionSuccess(
        NOTARY_ID, NYM_ID, ACCOUNT_ID, strMessage);
    switch (nSuccess) {
    case -1:
        otOut << "VerifyMsgTrnxSuccess: Error calling "
                 "OT_API_Message_GetSuccess, for message:\n\n" << strMessage
              << "\n";
        break;
    case 0:
        otWarn << "VerifyMsgTrnxSuccess: Reply received: success == FALSE. "
                  "Reply message:\n\n" << strMessage << "\n";
        break;
    case 1:
        otWarn << "VerifyMsgTrnxSuccess: Reply received: success == TRUE.\n";
        break;
    default:
        otOut << "VerifyMsgTrnxSuccess: Error. (This should never happen!) "
                 "Input: " << strMessage << "\n";
        nSuccess = -1;
        break;
    }

    return nSuccess;
}

//
// This code was repeating a lot, so I just added a function for it.
//
OT_UTILITY_OT int32_t InterpretTransactionMsgReply(const string& NOTARY_ID,
                                                   const string& NYM_ID,
                                                   const string& ACCOUNT_ID,
                                                   const string& strAttempt,
                                                   const string& strResponse)
{
    int32_t nMessageSuccess = VerifyMessageSuccess(strResponse);
    if (-1 == nMessageSuccess) {
        otOut << "Message error: " << strAttempt << ".\n";
        return -1;
    }
    if (0 == nMessageSuccess) {
        otOut << "Server reply (" << strAttempt << "): Message failure.\n";

        return 0;
    }

    int32_t nBalanceSuccess = VerifyMsgBalanceAgrmntSuccess(
        NOTARY_ID, NYM_ID, ACCOUNT_ID, strResponse);
    if (-1 == nBalanceSuccess) {
        otOut << "Balance agreement error: " << strAttempt << ".\n";
        return -1;
    }
    if (0 == nBalanceSuccess) {
        otOut << "Server reply (" << strAttempt
              << "): Balance agreement failure.\n";
        return 0;
    }

    int32_t nTransSuccess =
        VerifyMsgTrnxSuccess(NOTARY_ID, NYM_ID, ACCOUNT_ID, strResponse);
    if (-1 == nTransSuccess) {
        otOut << "Transaction error: " << strAttempt << ".\n";
        return -1;
    }
    if (0 == nTransSuccess) {
        otOut << "Server reply (" << strAttempt << "): Transaction failure.\n";
        return 0;
    }

    return 1;
}

//// Provides the functionality of the ? : operator.
//// NOTE: the chai team have since added ? :, so this is deprecated now.
//
// OT_UTILITY_OT int32_t ifB(the_expression, X, Y)
//{
//    var theReturnValue
//
//    if (the_expression)
//    {
//        theReturnValue = X;
//    }
//    else
//    {
//        theReturnValue = Y;
//    }
//
//    // Returning...
//    //
//
//    return theReturnValue;
//
//}

////
///********************************************************************************
//
// attr OTBool::value
// attr OTBool::value2
//
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::OTBool()
//{
//    value = false;
//    value2 = false;
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::OTBool(param_value)
//{
//    value = param_value;
//    value2 = false;
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::getBooleanValue()
//{
//
//    return value;
//
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::setBooleanValue(param_value)
//{
//    value = param_value;
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::getSecondValue()
//{
//
//    return value2;
//
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTBool::setSecondValue(param_value)
//{
//    value2 = param_value;
//}
//
////
///********************************************************************************
//
// attr OTInteger::value
//
//// ----------------------------
// OT_UTILITY_OT int32_t OTInteger::OTInteger()
//{
//    value = 0;
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTInteger::OTInteger(param_value)
//{
//    value = param_value;
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTInteger::getIntegerValue()
//{
//    // Returning...
//    //
//
//    return value;
//
//}
//// ----------------------------
// OT_UTILITY_OT int32_t OTInteger::setIntegerValue(param_value)
//{
//    value = param_value;
//}
////
///********************************************************************************

Utility::Utility()
    : strLastReplyReceived("")
    , delay_ms(50)
    , max_trans_dl(10)
{
}

Utility::~Utility()
{
}

OT_UTILITY_OT void Utility::delay() const
{
    OTAPI_Wrap::Sleep(delay_ms);
}

OT_UTILITY_OT void Utility::longDelay() const
{
    OTAPI_Wrap::Sleep(delay_ms + 200);
}

OT_UTILITY_OT int32_t Utility::getNbrTransactionCount() const
{
    return max_trans_dl;
}

OT_UTILITY_OT void Utility::setNbrTransactionCount(int32_t new_trans_dl)
{
    max_trans_dl = new_trans_dl;
}

OT_UTILITY_OT string Utility::getLastReplyReceived() const
{
    return strLastReplyReceived;
}

OT_UTILITY_OT void Utility::setLastReplyReceived(const string& strReply)
{
    strLastReplyReceived = strReply;
}

OT_UTILITY_OT int32_t
    Utility::getNymboxLowLevel(const string& notaryID, const string& nymID)
{
    bool bWasSent = false;
    return getNymboxLowLevel(notaryID, nymID, bWasSent);
}

// This returns -1 if error, or a positive request number if it was sent.
// (It cannot return 0;
// Called by getAndProcessNymbox.
//
OT_UTILITY_OT int32_t Utility::getNymboxLowLevel(const string& notaryID,
                                                 const string& nymID,
                                                 bool& bWasSent)
{
    string strLocation = "Utility::getNymboxLowLevel";

    OTAPI_Wrap::FlushMessageBuffer();
    bWasSent = false;

    int32_t nRequestNum = OTAPI_Wrap::getNymbox(
        notaryID, nymID); // <===== ATTEMPT TO SEND THE MESSAGE HERE...;

    if (OTAPI_Wrap::networkFailure()) {
        otOut << strLocation
        << ": getNymbox message failed due to network error.\n";
        return -1;
    }
    if (-1 == nRequestNum) {
        otOut << strLocation
        << ": Failed to send getNymbox message due to error.\n";
        return -1;
    }
    if (0 == nRequestNum) {
        otOut << strLocation << ": Unexpectedly returned 0. Didn't send "
                                "getNymbox message, but NO error occurred, "
                                "either. (In this case, SHOULD NEVER HAPPEN. "
                                "Treating as Error.)\n";
        return -1; // Even though '0' MEANS "didn't send, but no error" by
                   // convention in many places, it is actually an impossible
                   // return value;
    }
    if (nRequestNum < 0) {
        otOut << strLocation << ": Unexpected request number: " << nRequestNum
              << "\n";
        return -1;
    }

    bWasSent = true;

    //
    int32_t nResult =
        receiveReplySuccessLowLevel(notaryID, nymID, nRequestNum, strLocation);

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": Failed to receiveReplySuccessLowLevel due to network error.\n";
        return -1;
    }

    //      otOut << strLocation << ": receiveReplySuccessLowLevel:
    // " << nResult << "\n";

    // BY this point, we definitely have the request number in nResult, which
    // means
    // the message was actually SENT. (At least.) This also means we can use
    // nResult
    // later to query for a copy of that sent message.
    // Let's go ahead, in this case, and remove that now:
    //
    // THE REMOVE SENT MESSAGE BELOW FAILS, LIKE IT'S ALREADY GONE.
    //
    // THIS MUST BE DUE TO THE PROCESS SERVER REPLY THAT OCCURS **IMMEDIATELY**
    // after the message was originally sent!
    // (The reply came in and was sent to OT's "ProcessServerReply", INSIDE the
    // call to getNymbox.)
    // Our subsequent "receive" (above) is nothing of the sort, but actually
    // pops the incoming message buffer where
    // the server's reply was ALREADY SITTING, since it was put there in OT's
    // "ProcessServerReply", WHICH REMOVED THE
    // SENT MESSAGE ALREADY (that's why the below call to RemoveSentMessage
    // fails.)
    //
    // RETHINK any logic that doesn't take this into account,.
    // Either we REMOVE this call wherever this happens, OR... we call Get first
    // and make sure whether it's
    // there, THEN remove it. But we can't be lumping "Failure because it's
    // gone" versus "Error state" by mixing
    // 0 and -1 here. We need to differentiate.
    //
    // Bottom line: if the reply WAS received, then the original sent message
    // has ALREADY been removed
    // from the sent buffer. Whereas if the reply was NOT received, then the
    // sent message is still there,
    // but in that case, we do NOT want to remove it -- we want it to STAY in
    // the sent buffer, so that
    // when we get the Nymbox later and we DO have the reply from that, THEN we
    // can remove the sent msg from
    // the sent buffer. Until then, we don't want OT to think it's already been
    // processed (which it will, if
    // it's already been removed from the sent buffer. So we leave it there for
    // now.)
    //

    //      int32_t nRemovedSentMsg =
    // OTAPI_Wrap::RemoveSentMessage(to_string(nRequestNum), notaryID, nymID);
    //
    //      if (nRemovedSentMsg < 1)
    //      {
    //          otOut << strLocation << ": ERROR:
    // OT_API_RemoveSentMessage returned: " << nRemovedSentMsg << "\n";
    //      }

    if (1 == nResult) {
        return nRequestNum;
    }

    return nResult;
}

OT_UTILITY_OT int32_t
    Utility::getNymbox(const string& notaryID, const string& nymID)
{
    bool bForceDownload = false;
    return getNymbox(notaryID, nymID, bForceDownload);
}

OT_UTILITY_OT int32_t Utility::getNymbox(const string& notaryID,
                                         const string& nymID,
                                         bool bForceDownload)
{
    string strLocation = "Utility::getNymbox";

    string strRecentHash = OTAPI_Wrap::GetNym_RecentHash(notaryID, nymID);
    bool bRecentHash = VerifyStringVal(strRecentHash);
    if (!bRecentHash) {
        otOut << strLocation << ": Warning: Unable to retrieve recent cached "
                                "copy  of server-side NymboxHash from "
                                "client-side nym (perhaps he's never "
                                "downloaded it before?)\n\n";
    }

    string strLocalHash = OTAPI_Wrap::GetNym_NymboxHash(notaryID, nymID);
    bool bLocalHash = VerifyStringVal(strLocalHash);
    if (!bLocalHash) {
        otOut << strLocation << ": Warning: Unable to retrieve client-side "
                                "NymboxHash for:\n notaryID: " << notaryID
              << "\n nymID: " << nymID << "\n";
    }

    if (!bForceDownload) {
        if (bLocalHash && bRecentHash &&
            (strRecentHash == strLocalHash)) // the hashes match -- no need to
                                             // download anything.
        {
            otWarn
                << strLocation
                << ": The hashes already match (skipping Nymbox download.)\n";
            return 1;
        }
    }

    // -- SECTION 1: "GET NYMBOX"
    //
    bool bWasMsgSent = false;
    int32_t nGetNymbox = getNymboxLowLevel(
        notaryID, nymID, bWasMsgSent); // bWasMsgSent is output from this call.;

    if (OTAPI_Wrap::networkFailure())
        return -1;

    if (bWasMsgSent) {
        otWarn << strLocation
               << ": FYI: we just sent a getNymboxLowLevel msg. RequestNum: "
               << nGetNymbox << "\n";
    }

    if (!(bWasMsgSent) || ((nGetNymbox <= 0) && (-1 != nGetNymbox))) {
        otOut << strLocation
              << ": Failure: this.getNymboxLowLevel returned unexpected value: "
              << nGetNymbox << "\n";
        return -1;
    } // NOTE: for getNymbox, there is no '0' return value;

    if (-1 ==
        nGetNymbox) // we'll try re-syncing the request number, then try again.
    {
        otOut << strLocation
              << ": FYI: this.getNymboxLowLevel returned -1. (Re-trying...)\n";

        int32_t nGetRequestNumber = getRequestNumber(notaryID, nymID);
        if (1 != nGetRequestNumber) {
            otOut << strLocation << ": Failure: this.getNymboxLowLevel failed, "
                                    "then I tried to resync with "
                                    "this.getRequestNumber and then that "
                                    "failed too. (I give up.)\n";
            return -1;
        }

        // todo: should the member variable strLastReplyReceived be set to this
        // one?
        // before, the member var was shadowed (string strLastReplyReceived =
        // getLastReplyReceived();).
        string lastReplyReceived = getLastReplyReceived();
        // I had to do this bit because getRequestNumber doesn't return the
        // reply itself. But in this case, I needed it.
        if (!VerifyStringVal(lastReplyReceived)) // THIS SHOULD NEVER HAPPEN.
        {
            otOut << strLocation << ": ERROR in getLastReplyReceived(): why "
                                    "was this string not set, when "
                                    "this.getRequestNumber was otherwise an "
                                    "apparent success?\n";
            return -1; // (SHOULD NEVER HAPPEN. This string is set in the
                       // getRequestNumber function.)
        }

        // BY THIS POINT, we have received a server reply:
        // getRequestNumberResponse
        // (Unless it is malformed.) It's definitely not null, nor empty.

        // Grab the NymboxHash on the getRequestNumberResponse reply, and also
        // the one
        // I
        // already had on my client-side Nym... (So we can compare them.)
        //
        //      If the hashes do NOT match, then I DO need to download nymbox
        // and box receipts.
        /*
        *      ===> If the NymboxHash is changed from what I expected, then I
        *need to re-download the
        *      nymbox (and any box receipts I don't already have.)
        *
        *      Then I need to process the Nymbox. But first, see if my missing
        *server reply is in there.
        *      If it is, then I have the server reply! (As if we had succeeded
        *in the first place!!)
        *      Next, process the Nymbox (which processes that reply) and then
        *return strReply;
        *
        *      (Clearly this is just going to be a normal part of the
        *getRequestNumber
        *syncronization.)
        *
        *      By the time that much is done, I will KNOW the request number,
        *the nymbox, the box receipts,
        *      etc are ALL syncronized properly, and that I THEN processed the
        *Nymbox successfully.
        *
        *
        *      NOTICE: In this example I do NOT want to pull out my sent message
        *from the outbuffer (using
        *      the request number) and try to harvest all the transaction
        *numbers. Why not? Because possibly the
        *      server DID reply! And if I processed that reply properly, it
        *would sync my transaction numbers
        *      properly just from that! ===>
        *
        *      ===> Therefore, I need to see FIRST if the old message has a
        *reply WAITING in the Nymbox. THEN
        *      I need to process the Nymbox. ONLY if the reply wasn't there, can
        *I THEN pull out the message
        *      from my outbuffer and harvest it. (Which I am reticent to do,
        *until I am SURE the server
        *      really never saw that message in the first place.)
        *
        *      However, as long as my NymboxHash hasn't changed, then I'm safe!
        *But if it HAS changed,
        *      then I HAVE to A. download it B. SEE if the reply is there for
        *the request number, then
        *      C. process it. ... If the reply wasn't there, THEN Harvest the
        *transaction #s (for transaction
        *      messages) and then re-try.
        */

        // Grabbing again in case it's changed.
        string strServerHash =
            OTAPI_Wrap::Message_GetNymboxHash(lastReplyReceived);
        bool bServerHash = VerifyStringVal(strServerHash);
        if (!bServerHash) {
            otOut << strLocation
                  << ": Warning: Unable to retrieve server-side NymboxHash "
                     "from server getRequestNumberResponse reply:\n\n"
                  << lastReplyReceived << "\n";
        }

        strLocalHash = OTAPI_Wrap::GetNym_NymboxHash(notaryID, nymID);
        bLocalHash = VerifyStringVal(strLocalHash);
        if (!bLocalHash) {
            otOut << strLocation
                  << ": Warning(2): Unable to retrieve client-side NymboxHash "
                     "for:\n notaryID: " << notaryID << "\n nymID: " << nymID
                  << "\n";
        }

        // The hashes don't match -- so let's definitely re-try to download the
        // latest nymbox.
        if (bForceDownload || !bLocalHash || !bServerHash ||
            (bServerHash && bLocalHash && !(strServerHash == strLocalHash))) {
            // the getRequestNumber worked, and the server hashes don't match,
            // so let's try the call again...

            nGetNymbox = getNymboxLowLevel(notaryID, nymID, bWasMsgSent);

            if (!(bWasMsgSent) || ((nGetNymbox <= 0) && (-1 != nGetNymbox))) {
                otOut << strLocation
                      << ": Failure(2): this.getNymboxLowLevel returned "
                         "unexpected value: " << nGetNymbox << "\n";
                return -1;
            }

            if (-1 == nGetNymbox) // we'll try re-syncing the request number,
                                  // then try again.
            {
                otOut << strLocation << ": Failure: this.getNymboxLowLevel "
                                        "returned -1, even after syncing the "
                                        "request number successfully. (Giving "
                                        "up.)\n";
                return -1;
            }
        }
    }

    // By this point, we DEFINITELY know that the Nymbox was retrieved
    // successfully.
    // (With request number nGetNymbox.) This is because the getNymboxLowLevel()
    // call
    // also tries to receive the reply, so we already know by now whether the
    // reply
    // was successfully received.
    //

    return nGetNymbox;
}

// NEW ONES:

// public static int32_t getAndProcessNymbox(String notaryID, String nymID,
// OTBool bWasMsgSent, boolean bForceDownload,
//                                      var nRequestNumber, // nRequestNumber
// refers to a PREVIOUS msg (like a cash withdrawal) that had an error and then
// called this while trying to resync. (The caller will want to know whether it
// was found in the Nymbox.)
//                                      OTBool bFoundNymboxItem,  //
// bFoundNymboxItem is OUTPUT bool, telling caller whether it was found.
//                                      boolean bHarvestingForRetry, //
// bHarvestingForRetry is INPUT, in the case nRequestNumber needs to be
// harvested before a flush occurs.
//                                      boolean bMsgReplySuccess,    //
// bMsgReplySuccess is INPUT, and is in case nRequestNumber needs to be
// HARVESTED before a FLUSH happens.
//                                      boolean bMsgReplyFailure,    //
// bMsgReplyFailure is INPUT, and is in case nRequestNumber needs to be
// HARVESTED before a FLUSH happens.
//                                      boolean bMsgTransSuccess,    //
// bMsgTransSuccess is INPUT, and is in case nRequestNumber needs to be
// HARVESTED before a FLUSH happens.
//                                      boolean bMsgTransFailure)    //
// bMsgTransFailure is INPUT, and is in case nRequestNumber needs to be
// HARVESTED before a FLUSH happens.

// def Utility::getAndProcessNymbox_8(notaryID, nymID, bWasMsgSent, //
// bWasMsgSent is OUTPUT.
//                                   bForceDownload,       // INPUT
//                                   nRequestNumber,       // nRequestNumber
// refers to a PREVIOUS msg (like a cash withdrawal) that had an error and then
// called this while trying to resync. (The caller will want to know whether it
// was found in the Nymbox.)
//                                   bFoundNymboxItem,     // bFoundNymboxItem
// is OUTPUT bool, telling caller whether it was found.
//                                   bHarvestingForRetry,  //
// bHarvestingForRetry is INPUT, in the case nRequestNumber needs to be
// harvested before a flush occurs.
//                                   bMsgFoursome) // OTfourbool class, used to
// reduce the number of parameters here, which I am suspicious that ChaiScript
// cannot handle.

// Returns:
//   -1 ERROR.
//    0 Nymbox was empty -- nothing done. (bWasMsgSent = false)
//    0 Transactionstatus = = server reply received (bWasMsgSent = true), but;
//      the server reply hasstatus = = FAILED. (All harvesting was subsequently
// successful for processNymbox).;
//    1 If the ProcessNymbox Transaction status (from the server reply) is
// SUCCESS,
//      then this function returns 1.
//   >1 If the ProcessNymbox Transaction status (from the server reply) is >1,
// then this function returns the
//      REQUEST NUMBER from when it was originally sent. (Harvesting was NOT
// performed, which is why the request
//      number is being returned, so the caller can choose what to do next.)

// def Utility::getAndProcessNymbox_8(notaryID, nymID, bWasMsgSent,
// bForceDownload, nRequestNumber, bFoundNymboxItem, bHarvestingForRetry,
// bMsgFoursome)
//{
//    cout << "Here's the dead version!\n";
//}

// var nGetAndProcessNymbox = getAndProcessNymbox_8(notaryID, nymID,
// bWasMsgSent, bForceDownload, nRequestNumber, bFoundNymboxItem,
// bHarvestingForRetry, the_foursome);

OT_UTILITY_OT int32_t Utility::getAndProcessNymbox_8(
    const string& notaryID, const string& nymID, bool& bWasMsgSent,
    bool bForceDownload, int32_t nRequestNumber, bool& bFoundNymboxItem,
    bool bHarvestingForRetry, const OTfourbool& bMsgFoursome)
{
    string strLocation = "Utility::getAndProcessNymbox";

    bool bMsgReplySuccess = bMsgFoursome[0];
    bool bMsgReplyFailure = bMsgFoursome[1];
    bool bMsgTransSuccess = bMsgFoursome[2];
    bool bMsgTransFailure = bMsgFoursome[3];

    if (0 > nRequestNumber) {
        otOut << "\n\n\n\n Failed verifying nRequestNumber as an integer. "
                 "\n\n\n\n\n";
        return -1;
    }

    if (1 == nRequestNumber) {
        otOut << strLocation
              << ": WARNING: Request Num of '1' was just passed in here.\n";
    }

    bWasMsgSent = false;

    // what is nRequestNumber?
    //
    // Let's say a message, say for a cash withdrawal with request number 5, has
    // FAILED.
    // Since the message failed, perhaps the request number was out of sync, or
    // Nymbox hash
    // was old? So, let's say that it then sent a getRequestNumber message, in
    // order
    // to resync,
    // and discovered that the Nymbox hash has changed. Therefore the Nymbox is
    // now being
    // re-downloaded and processed, so that the cash withdrawal can be attempted
    // again.
    //
    // HOWEVER, before we PROCESS the Nymbox, we need to see if the withdrawal
    // reply is already
    // sitting in it. Why, you ask, if the withdrawal failed, would I expect a
    // reply to be in
    // the Nymbox? In case 1, the message was dropped, so I don't know if the
    // reply is there
    // until I check the Nymbox. In case 2, the message may have failed OR
    // SUCCEEDED, with the
    // successful message containing a FAILED TRANSACTION.
    // Thus, we just want to check the Nymbox for nRequestNumber, and make sure
    // whether it's
    // there or not, before we PROCESS the nymbox, because once we do THAT, it
    // will be empty.
    //
    // We will return a;
    // was already in the Nymbox. We can also harvest the transaction numbers
    // from the reply
    // message, if it's a transaction, so that everything is set for the re-try.
    // (Possibly pass
    // a bool parameter dictating whether the harvesting is being done for a
    // re-try or not.)
    //

    // -- SECTION 1: "GET NYMBOX"
    //
    // This call is sufficiently high-level enough that it already has re-tries
    // built into it. That's why you don't see me re-trying the getNymbox if it
    // fails.
    //

    int32_t nGetNymbox = getNymbox(notaryID, nymID, bForceDownload);
    if (nGetNymbox < 1) {
        otOut << strLocation
              << ": Failure: this.getNymbox returned: " << nGetNymbox << "\n";
        return -1;
    }

    // By this point, we DEFINITELY know that the Nymbox was retrieved
    // successfully.
    // (With request number nGetNymbox.) This is because the getNymboxLowLevel()
    // call
    // also tries to receive the reply, so we already know by now whether the
    // reply
    // was successfully received.
    //
    /*
    *  FYI: nRequestNumber is the request number, if >0, for whatever command
    * is causing this getAndProcessNymbox to occur (like a cash withdrawal, or
    * a cheque deposit, etc.) We pass it in here so we can verify whether it's
    *on
    * the Nymbox, before we process it out (so the caller knows whether to
    *clawback.)
    *
    * FYI: nGetNymbox is the request number from getNymboxLowLevel() (above.) We
    * know for a FACT, by this point, this number is >0.
    *
    * FYI: nProcess (just below) is the request number for the PROCESS NYMBOX
    *message.
    * Below the switch block just down there, we know for a fact this number is
    *>0.
    */

    // -- SECTION 2: "SEND PROCESS NYMBOX"
    //

    // Next, we have to make sure we have all the BOX RECEIPTS downloaded
    // for this Nymbox.

    // If the caller wanted to know whether a certain reply (by request number)
    // was in the Nymbox, then bFoundNymboxItem
    // will be set true in this call, if it was there. That way he can Harvest
    // his own msg if he needs to. (Just like I
    // harvest my own processNymbox call below, if necessary.)
    //
    // nBoxType = 0 aka nymbox;
    //

    int32_t nBoxType = 0;

    bool bInsured = insureHaveAllBoxReceipts(
        notaryID, nymID, nymID, nBoxType, nRequestNumber,
        bFoundNymboxItem); // ***************************;
    if (bInsured) {
        // If the caller was on about a specific request number...
        //
        if (nRequestNumber > 0) {
            // ...And if we DID NOT find that request number in the Nymbox
            // (along with the server's reply), then harvest it!!
            // (If we HAD found it, then we'd know it didn't NEED harvesting,
            // since the server clearly REPLIED to it already.)

            // FOUND it in the nymbox! Therefore we can remove without
            // harvesting.
            // (Server definitely processed it, so nothing to harvest.)
            //
            if (bFoundNymboxItem) {
                // Notice, if the above call to insureHaveAllBoxReceipts had any
                // network hiccups, then
                // it may have had to get and processNymbox, meaning the below
                // Remove would fail, since
                // the sent message was already removed. Therefore, might want
                // to update this to call getSent
                // FIRST, before trying to remove.
                // (Might want different messages in either case.)
                //
                bool nRemovedMsg = OTAPI_Wrap::RemoveSentMessage(
                    int64_t(nRequestNumber), notaryID, nymID);
                otInfo << strLocation
                       << ": OT_API_RemoveSentMessage: (Found server reply in "
                          "Nymbox. Removing local sent msg.) Request number: "
                       << nRemovedMsg << "\n";
            }
            else // We DIDN'T find it in the nymbox, so we can harvest it:
            {
                // NOTE: This may always fail,

                otLog3 << strLocation
                       << ": FYI: Calling OT_API_GetSentMessage...\n";

                string strSentMsg = OTAPI_Wrap::GetSentMessage(
                    int64_t(nRequestNumber), notaryID, nymID);

                if (!VerifyStringVal(strSentMsg)) {
                    otInfo << strLocation
                           << ": (1) OT_API_GetSentMessage returned nullptr "
                              "for clawback. (Must have already been cleared. "
                              "OT uses deliberate redundancy, though optimizes "
                              "this wherever possible.) Request number: "
                           << nRequestNumber << "\n";
                }
                else // OTAPI_Wrap::GetSentMessage success.
                {
                    otOut << strLocation << ": FYI: Harvesting transaction "
                                            "numbers from failed Msg "
                                            "attempt...\n";

                    bool nHarvested = OTAPI_Wrap::Msg_HarvestTransactionNumbers(
                        strSentMsg, nymID,
                        bHarvestingForRetry, // bHarvestingForRetry.
                        bMsgReplySuccess, // bReplyWasSuccess,       // RECEIVED
                                          // server reply: explicit success.
                        bMsgReplyFailure, // bReplyWasFailure,       // RECEIVED
                                          // server reply: explicit failure.
                        bMsgTransSuccess, // bTransactionWasSuccess, // MESSAGE
                                          // success, Transaction success.
                                          // (Explicit.)
                        bMsgTransFailure); // bTransactionWasFailure  // MESSAGE
                                           // success, Transaction failure.
                                           // (Explicit.)
                    otOut << strLocation
                          << ": OT_API_Msg_HarvestTransactionNumbers: "
                          << nHarvested << "\n";

                    bool nRemovedMsg = OTAPI_Wrap::RemoveSentMessage(
                        int64_t(nRequestNumber), notaryID, nymID);
                    otInfo << strLocation
                           << ": OT_API_RemoveSentMessage: " << nRemovedMsg
                           << "\n";
                } // strSentMsg NOT null!
            }
        }

        // (flush): LOOP THROUGH ALL "SENT" messages, and see if ANY of them has
        // a reply
        // sitting in my Nymbox. If so, REMOVE IT from "Sent" queue, (since
        // clearly the server
        // DID respond already.) And if it's NOT in my nymbox, that means I
        // DEFINITELY need to
        // harvest it back since the server definitely rejected it or never
        // received it.
        //
        // The Nym actually SAVES the sent messages PER SERVER, so that this
        // will continue to work in every possible case!!
        // NOTE: Also now storing, on the client nym, a copy of the server's
        // latest nymbox hash
        // for that nym, in addition to the nym's copy (which only updates when
        // he gets his Nymbox.)
        // That way the Nym, even before syncing the nymboxes, and even before
        // sending a new message
        // to find out if they are out of sync, ALREADY KNOWS if they're in sync
        // or not. (That's why
        // all those other server messages send a copy of that hash back, not
        // just the getNymbox msg.)
        //
        //
        //            void OTAPI_Wrap::FlushSentMessages( int32_t //
        // bHarvestingForRetry, // bHarvestingForRetry is actually OT_BOOL
        //                              const char * NOTARY_ID,
        //                              const char * NYM_ID,
        //                              const char * THE_NYMBOX);
        // NoVerify means don't load up all the box receipts.
        // Especially in this case--we only care about whether a box receipt is
        // THERE, not
        // what it contains. FlushSentMessages will work just fine WITHOUT
        // loading those
        // box receipts (because the Nymbox contains enough of an abbreviated
        // record already
        // for each one, that we will have the info we need already.)
        //
        string strNymbox = OTAPI_Wrap::LoadNymboxNoVerify(
            notaryID, nymID); // FLUSH SENT MESSAGES!!!!  (AND HARVEST.);

        if (VerifyStringVal(strNymbox)) {

            OTAPI_Wrap::FlushSentMessages(
                false, // harvesting for retry = = OT_FALSE. None of the things
                       // are being re-tried by the time they are being flushed.
                       // They were already old news.;
                notaryID, nymID, strNymbox);
        }
        // Flushing removes all the messages from the "sent messages" queue,
        // and harvests any transaction numbers to be had. How do I know for
        // sure
        // that I can get away with this? How do I know whether the server has
        // processed those messages or not? How can I harvest them as though
        // they
        // were dropped on the network somewhere? The answer is because I JUST
        // called GetNymbox and downloaded the latest one. I can SEE which
        // replies
        // are in there -- and which ones aren't. I pass that Nymbox into the
        // flush
        // call, so that flush can be careful to remove all sent messages that
        // have
        // nymbox replies, and only harvest the others.
        else {
            otOut << strLocation
                  << ": Error while trying to flush sent messages: Failed "
                     "loading Nymbox for nym: " << nymID << "\n";
        }

        int32_t nMsgSentRequestNumOut = -1;
        int32_t nReplySuccessOut = -1;
        int32_t nBalanceSuccessOut = -1;
        int32_t nTransSuccessOut = -1;

        // PROCESS NYMBOX
        //
        // Returns:
        // -1 Error.
        //  0 Nymbox was empty -- nothing done. (bWasMsgSent = false)
        //  0 Transactionstatus = = server reply received (bWasMsgSent = true),;
        //    but the server reply saysstatus = =failed.;
        // >0 If the Transaction status (from the server reply) is SUCCESS, then
        // this function
        //    returns the REQUEST NUMBER from when it was originally sent.

        int32_t nProcess = processNymbox(
            notaryID, nymID, bWasMsgSent, nMsgSentRequestNumOut,
            nReplySuccessOut, nBalanceSuccessOut, nTransSuccessOut);

        if (-1 == nProcess) {
            // Todo: might want to remove the sent message here, IF bMsgWasSent
            // is true.
            // (Just like case 0.)
            //
            otOut << strLocation << ": Failure: processNymbox: error (-1). (It "
                                    "couldn't send. I give up.)\n";

            return -1; // (It didn't even send.)
        }
        else if (0 == nProcess) {
            // Nymbox was empty. (So we didn't send any process message because
            // there was nothing to process.)
            if (!bWasMsgSent) {
                return 0; // success. done. (box was empty already.)
            }
            // else: the message WAS sent, (the Nymbox was NOT empty)
            //       and then the server replied "success==FALSE"
            //       in its REPLY to that message! Thus we continue and DROP
            // THROUGH...
        }
        else if (nProcess < 0) {
            otOut << strLocation
                  << ": Failure: processNymbox: unexpected: " << nProcess
                  << ". (I give up.)\n";

            return -1;
        }
        // bWasMsgSent = true;  // unnecessary -- set already by processNymbox
        // call above.

        // By this point, we definitely have a >0 request number from the
        // sendProcessNymbox()
        // call, stored in nProcess (meaning the message WAS sent.) (Except in
        // case of 0, see next line which fixes this:)
        //

        nProcess = nMsgSentRequestNumOut; // Sometimes this could be 0 still, so
                                          // we fix it here.;
        int32_t nReplySuccess = nReplySuccessOut;
        int32_t nTransSuccess = nTransSuccessOut;
        int32_t nBalanceSuccess = nBalanceSuccessOut;

        /*

        return const;

        char *    OTAPI_Wrap::GetSentMessage(const char* REQUEST_NUMBER)
        OT_BOOL   OTAPI_Wrap::RemoveSentMessage(const char* REQUEST_NUMBER)

        */
        // All of these booleans (except "error") represent RECEIVED ANSWERS
        // from the server.
        // In other words, "false" does not mean "failed to find message."
        // Rather, it means "DEFINITELY got a reply, and that reply sayssuccess
        // = =false.";

        // SHOULD NEVER HAPPEN (processNymbox call just above was successful,
        // therefore the sent message SHOULD be here in my cache.)
        //
        string strReplyProcess = getLastReplyReceived();
        // I had to do this bit because getRequestNumber doesn't return the;
        // reply itself. But in this case, I needed it.
        if (!VerifyStringVal(strReplyProcess)) // THIS SHOULD NEVER HAPPEN.
        {
            otOut << strLocation << ": ERROR in getLastReplyReceived(): why "
                                    "was this string not set, when "
                                    "getRequestNumber was otherwise an "
                                    "apparent success?\n";

            return -1; // (SHOULD NEVER HAPPEN. This string is set in the
                       // getRequestNumber function.)
        }

        bool bProcessNymboxReplyError =
            (!VerifyStringVal(strReplyProcess) || (nReplySuccess < 0));
        bool bProcessNymboxBalanceError =
            (!VerifyStringVal(strReplyProcess) || (nBalanceSuccess < 0));
        bool bProcessNymboxTransError =
            (!VerifyStringVal(strReplyProcess) || (nTransSuccess < 0));

        bool bProcessNymboxReplySuccess =
            (!bProcessNymboxReplyError && (nReplySuccess > 0));
        bool bProcessNymboxReplyFailure =
            (!bProcessNymboxReplyError && (nReplySuccess == 0));

        bool bProcessNymboxBalanceSuccess =
            (!bProcessNymboxReplyError && !bProcessNymboxBalanceError &&
             (nBalanceSuccess > 0));
        bool bProcessNymboxBalanceFailure =
            (!bProcessNymboxReplyError && !bProcessNymboxBalanceError &&
             (nBalanceSuccess == 0));

        bool bProcessNymboxTransSuccess =
            (!bProcessNymboxReplyError && !bProcessNymboxBalanceError &&
             !bProcessNymboxTransError && (nTransSuccess > 0));
        bool bProcessNymboxTransFailure =
            (!bProcessNymboxReplyError && !bProcessNymboxBalanceError &&
             !bProcessNymboxTransError && (nTransSuccess == 0));

        bool bProcessAnyError =
            (bProcessNymboxReplyError || bProcessNymboxBalanceError ||
             bProcessNymboxTransError);
        bool bProcessAnyFailure =
            (bProcessNymboxReplyFailure || bProcessNymboxBalanceFailure ||
             bProcessNymboxTransFailure);
        bool bProcessAllSuccess =
            (bProcessNymboxReplySuccess && bProcessNymboxBalanceSuccess &&
             bProcessNymboxTransSuccess);

        // Note: we LEAVE the sent message in the "sent queue" until we are
        // certain that it processed.
        // If we are NOT certain that it processed, then we try to download the
        // Nymbox and see if there's
        // a reply there (for the sent message.) If we are able to confirm THAT,
        // AFTER SUCCESSFULLY downloading
        // the Nymbox, then then we don't have to do anything because we know
        // for sure it was processed.
        // Similarly, if we DEFINITELY download the Nymbox and do NOT find the
        // reply, then we know the server
        // DEFINITELY did not receive (or at least process) that message, which
        // is what allows us to HARVEST
        // the transaction numbers back from the sent message, and remove the
        // sent message from the sent queue.
        //
        // However, if we are NOT able to Verify any of these things, NOR are we
        // able to download the Nymbox to
        // see, then we DO leave the message in the sent queue. This is
        // deliberate, since it gives us the opportunity
        // in the future to clear those sent messages NEXT time we successfully
        // DO download the Nymbox, and in the
        // meantime, it allows us to store a record of EXACTLY which messages
        // were MISSED.
        //

        int32_t nHarvested = -1;

        if (bProcessAllSuccess) {
            // the processNymbox was a complete success, including the message
            // AND the transaction AND the transaction statement.
            // Therefore, there's DEFINITELY nothing to clawback.
            //
            // (Thus I RemoveSentMessage for the processNymbox message, since
            // I'm totally done with it now.)
            //
            //         int32_t nRemoved =
            // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nProcess),
            // notaryID, nymID);

            // NOTE: The above call is unnecessary, since a successful process
            // means
            // we already received the successful server reply, and OT's
            // "ProcessServerReply"
            // already removed the sent message from the sent buffer (so no need
            // to do that here.)
            //
        }
        else if (bProcessAnyError ||
                   bProcessAnyFailure) // let's resync, and clawback whatever
                                       // transaction numbers we might have used
                                       // on the processNymbox request...
        {
            nGetNymbox = getNymbox(notaryID, nymID,
                                   true); // bForceDownload=true - NOTE: could
                                          // maybe change this to false and have
                                          // it still work.;

            if (nGetNymbox < 1) {
                otOut << strLocation
                      << ": Failure: this.getNymbox returned: " << nGetNymbox
                      << "\n";

                return -1;
            }

            bool bWasFound = false;
            nBoxType =
                0; // I think it was already zero, but since i'm using it here;
            // PURELY to be a '0', I feel safer setting it again. (Nymbox is 0.)
            //   bool bInsured = insureHaveAllBoxReceipts(notaryID, nymID,
            // nymID, nBoxType, nRequestNumber, bFoundNymboxItem);   //
            // ***************************;
            if (insureHaveAllBoxReceipts(notaryID, nymID, nymID, nBoxType,
                                         nProcess, bWasFound)) // This will tell
                                                               // us whether the
                                                               // processNymbox
                                                               // reply was
                                                               // found in the
                                                               // Nymbox
            {
                // we FOUND the processNymbox reply in the Nymbox!
                //
                if (bWasFound) {
                    // Thus, no need to clawback any transaction numbers,
                    // since the server clearly already processed this
                    // processNymbox
                    // transaction, since I have a reply to it already sitting
                    // in my Nymbox.
                    //
                    //                       int32_t nRemoved =
                    // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nProcess),
                    // notaryID, nymID);
                    //
                    // NOTE: The above call is unnecessary, since a successful
                    // process means
                    // we already received the successful server reply, and OT's
                    // "ProcessServerReply"
                    // already removed the sent message from the sent buffer (so
                    // no need to do that here.)

                    otOut << strLocation << ": FYI: I *did* find the "
                                            "processNymboxResponse reply in my "
                                            "Nymbox, so NO NEED to clawback "
                                            "any transaction numbers.\n";
                }
                else // was NOT found... we need to clawback.
                {
                    // This means the server's reply was definitely NOT found in
                    // the
                    // Nymbox, even after successfully DOWNLOADING that Nymbox.
                    // Which
                    // means the server never got it in the first place, or
                    // rejected it
                    // at the message level before the transaction portion had a
                    // chance
                    // to run. Either way, we need to claw back any relevant
                    // transaction
                    // numbers...

                    // HARVEST the processNymbox message from outgoing messages.

                    otLog3 << strLocation
                           << ": FYI: Calling OT_API_GetSentMessage...\n";

                    string strSentProcessNymboxMsg = OTAPI_Wrap::GetSentMessage(
                        int64_t(nProcess), notaryID, nymID);

                    if (!VerifyStringVal(strSentProcessNymboxMsg)) {
                        otInfo << strLocation
                               << ": (2) OT_API_GetSentMessage returned "
                                  "nullptr for clawback. (Must have already "
                                  "been cleared. OT uses deliberate "
                                  "redundancy, though optimizes this wherever "
                                  "possible.) Request number: " << nProcess
                               << "\n";
                    }
                    else // strSentProcessNymboxMsg NOT null!
                    {
                        otOut << strLocation << ": FYI: Harvesting transaction "
                                                "numbers from failed "
                                                "processNymbox attempt...\n";

                        nHarvested = OTAPI_Wrap::Msg_HarvestTransactionNumbers(
                            strSentProcessNymboxMsg, nymID,
                            false, // bHarvestingForRetry = = false;
                            bProcessNymboxReplySuccess, // bReplyWasSuccess,
                                                        // // RECEIVED server
                                                        // reply: explicit
                                                        // success.
                            bProcessNymboxReplyFailure, // bReplyWasFailure,
                                                        // // RECEIVED server
                                                        // reply: explicit
                                                        // failure.
                            bProcessNymboxTransSuccess, // bTransactionWasSuccess,
                                                        // // MESSAGE success,
                                                        // Transaction success.
                                                        // (Explicit.)
                            bProcessNymboxTransFailure); // bTransactionWasFailure
                                                         // // MESSAGE success,
                                                         // Transaction failure.
                                                         // (Explicit.)

                        otOut << strLocation
                              << ": OT_API_Msg_HarvestTransactionNumbers: "
                              << nHarvested << "\n";

                        bool nRemovedProcessNymboxMsg =
                            OTAPI_Wrap::RemoveSentMessage(int64_t(nProcess),
                                                          notaryID, nymID);

                        otInfo << strLocation << ": OT_API_RemoveSentMessage: "
                               << nRemovedProcessNymboxMsg << "\n";
                    } // strSentProcessNymboxMsg NOT null!
                }     // a specific receipt was not found in the nymbox (need to
                      // clawback the transaction numbers on that receipt.)

                strNymbox = OTAPI_Wrap::LoadNymboxNoVerify(
                    notaryID,
                    nymID); // FLUSH SENT MESSAGES!!!!  (AND HARVEST.);
                if (VerifyStringVal(strNymbox)) {
                    OTAPI_Wrap::FlushSentMessages(
                        false, // harvesting for retry = = OT_FALSE
                        notaryID, nymID, strNymbox);
                }
                // Flushing removes all the messages from the "sent messages"
                // queue,
                // and harvests any transaction numbers to be had. How do I know
                // for sure
                // that I can get away with this? How do I know whether the
                // server has
                // processed those messages or not? How can I harvest them as
                // though they
                // were dropped on the network somewhere? The answer is because
                // I JUST
                // called getNymbox and downloaded the latest one. I can SEE
                // which replies
                // are in there -- and which ones aren't. I pass that Nymbox
                // into the flush
                // call, so that flush can be careful to remove all sent
                // messages that have
                // nymbox replies, and only harvest the others.
                else {
                    otOut << strLocation << ": Error while trying to flush "
                                            "sent messages: Failed loading "
                                            "Nymbox for nym: " << nymID << "\n";
                }
            }    // if insureHaveAllBoxReceipts()
            else // we do NOT have all the box receipts.
            {
                otOut << strLocation << ": Error: insureHaveAllBoxReceipts "
                                        "failed. (I give up.)\n";
                return -1;
            }
        } // else if (bProcessAnyError || bProcessAnyFailure)

        // Return the request number, if potentially needed by caller.
        // If explicit success, the request number is returned as a positive
        // number (though already removed from sent queue.) Whereas if explicit
        // failure (replystatus = failed) then we harvest the numbers;
        //
        if (bProcessAllSuccess)
        //          return getNymbox(notaryID, nymID, true);
        // //bForceDownload = true. Since we DID process it successfully,
        // then we grab it again.
        {
            return 1;
        } // We don't need the sent message after this, and we've already
          // removed it from sent queue.

        if (bProcessAnyFailure || bProcessAnyError) {
            if (nHarvested < 1) // If the message failed, and the harvesting
                                // failed, then we return the
            {
                return nProcess;
            } // number for the process nymbox, so the caller has a choice of
              // what to do next.

            if (bProcessAnyFailure) {
                return 0;
            } // by this point, we've definitely harvested, AND removed sent
              // message from sent queue. So we just return 0;
        }

        return -1; // must've been an error.
    }              // if insureAllBoxReceipts()
    else {
        otOut << strLocation
              << ": insureHaveAllBoxReceipts failed. (I give up.)\n";
    }

    return -1;
}

//  def Utility::getAndProcessNymbox(String notaryID, String nymID, OTBool
// bWasMsgSent, boolean bForceDownload)
//
OT_UTILITY_OT int32_t
    Utility::getAndProcessNymbox_4(const string& notaryID, const string& nymID,
                                   bool& bWasMsgSent, bool bForceDownload)
{
    string strLocation = "Utility::getAndProcessNymbox_4";

    if (!VerifyStringVal(notaryID) || !VerifyStringVal(nymID)) {
        otOut << strLocation << ": SHOULD NEVER HAPPEN!!! ASSERT!! ERROR!! "
                                "FAILURE!!! PROBLEM!!!!!\n";
        return -1;
    }
    //   bool bMsgReplySuccess5 = false;
    //   bool bMsgReplyFailure5 = false;
    //   bool bMsgTransSuccess5 = false;
    //   bool bMsgTransFailure5 = false;

    int32_t nRequestNumber = 0;
    bool bHarvestingForRetry = false;
    OTfourbool the_foursome = {false, false, false, false};
    bool bFoundNymboxItem = false; // bFoundNymboxItem is output bool, telling
                                   // caller whether it was found.;

    return getAndProcessNymbox_8(notaryID, nymID, bWasMsgSent, bForceDownload,
                                 nRequestNumber, bFoundNymboxItem,
                                 bHarvestingForRetry, the_foursome);
}

//  public static int32_t getNymboxLowLevel(String notaryID, String nymID)
//  public static int32_t receiveNymboxLowLevel(String notaryID, String nymID,
// var nRequestNum)
//  public static int32_t processNymboxLowLevel(String notaryID, String nymID) {

//  def Utility::getAndProcessNymbox(String notaryID, String nymID, OTBool
// bWasMsgSent)
OT_UTILITY_OT int32_t Utility::getAndProcessNymbox_3(const string& notaryID,
                                                     const string& nymID,
                                                     bool& bWasMsgSent)
{
    bool bForceDownload = false;
    return getAndProcessNymbox_4(notaryID, nymID, bWasMsgSent, bForceDownload);
}

// NEW VERSION:

// PROCESS NYMBOX
//
// Returns:
// -1 Error.
//  0 Nymbox was empty -- nothing done. (bWasMsgSent = false)
//  0 server reply received, but it sayssuccess = =false on that msg.
// (bWasMsgSent = true);
// >0 If the Transaction status (from the server reply) is SUCCESS, then this
// function
//    returns the REQUEST NUMBER from when it was originally sent.
//
// public static int32_t processNymbox(String    notaryID, String nymID,
//                                OTBool    bWasMsgSent,
//
//                                OTInteger nMsgSentRequestNumOut,
//                                OTInteger nReplySuccessOut,
//                                OTInteger nBalanceSuccessOut,
//                                OTInteger nTransSuccessOut)

OT_UTILITY_OT int32_t
    Utility::processNymbox(const string& notaryID, const string& nymID,
                           bool& bWasMsgSent, int32_t& nMsgSentRequestNumOut,
                           int32_t& nReplySuccessOut,
                           int32_t& nBalanceSuccessOut,
                           int32_t& nTransSuccessOut)
{
    bWasMsgSent = false;
    string strLocation = "Utility::processNymbox";

    // if (!VerifyOTBoolRef(bWasMsgSent) ||
    // !VerifyOTIntegerRef(nReplySuccessOut) ||
    // !VerifyOTIntegerRef(nBalanceSuccessOut) ||
    // !VerifyOTIntegerRef(nTransSuccessOut))
    //{
    //    otOut << strLocation << ": SHOULD NEVER HAPPEN: has null
    // values passed in...\n";
    //    exit(-1)
    //}

    nMsgSentRequestNumOut = -1;
    nReplySuccessOut = -1;
    nBalanceSuccessOut = -1;
    nTransSuccessOut = -1;

    // -- SECTION 2: "SEND PROCESS NYMBOX"

    // Next, we have to make sure we have all the BOX RECEIPTS downloaded
    // for this Nymbox.

    int32_t nProcess = sendProcessNymboxLowLevel(
        notaryID, nymID); // <===================== SEND PROCESS NYMBOX!!;
    if (-1 == nProcess) {
        otOut << strLocation << "(2): error (-1), when calling "
                                "sendProcessNymboxLowLevel. (It couldn't send. "
                                "I give up.)\n";
        return -1; // (It didn't even send.)
    }
    // Nymbox was empty. (So we didn't send any process message because there
    // was nothing to process.)
    if (0 == nProcess) {
        return 0; // success. done.
    }
    if (nProcess < 0) {
        otOut << strLocation << ": unexpected: " << nProcess
              << ", when calling sendProcessNymboxLowLevel. (I give up.)\n";
        return -1;
    }

    bWasMsgSent = true;
    nMsgSentRequestNumOut = nProcess;

    // By this point, we definitely have a >0 request number from the
    // sendProcessNymbox()
    // call, stored in  ** nProcess ** (meaning the message WAS sent.)
    //
    // But was it received?
    //
    string strReplyProcess =
        ReceiveReplyLowLevel(notaryID, nymID, nProcess,
                             "processNymbox / sendProcessNymboxLowLevel / "
                             "ReceiveReplyLowLevel"); // <=============== Here
                                                      // we RECEIVE the REPLY...

    // getLastReplyReceived() will also contain the same as strReplyProcess.
    // So if the CALLER of this function (that we're in, receiveNymboxLowLevel)
    // wants to see the contents, he can.

    // ReceiveReplyLowLevel returns null unless there was a string returned.
    // So we can directly check it for success...

    int32_t nReplySuccess =
        VerifyMessageSuccess(strReplyProcess); // sendProcessNymboxLowLevel;
    int32_t nTransSuccess = -1;
    int32_t nBalanceSuccess = -1;
    ;
    if (nReplySuccess > 0) // If message was success, then let's see if the
                           // transaction was, too.
    {
        nBalanceSuccess = OTAPI_Wrap::Message_GetBalanceAgreementSuccess(
            notaryID, nymID, nymID,
            strReplyProcess); // the processNymbox transaction.;
        if (nBalanceSuccess > 0) {
            nTransSuccess = OTAPI_Wrap::Message_GetTransactionSuccess(
                notaryID, nymID, nymID,
                strReplyProcess); // the processNymbox transaction.;
        }
    }

    nReplySuccessOut = nReplySuccess;
    nBalanceSuccessOut = nBalanceSuccess;
    nTransSuccessOut = nTransSuccess;

    // NOTE: The caller MUST have a call to OTAPI_Wrap::RemoveSentMessage
    // to correspond to THIS function's call of sendProcessNymboxLowLevel().
    //
    if (nTransSuccess > 0) {
        return nProcess; // <=========================
    }

    return nTransSuccess;
}

// No need to deal with getRequestNumber here when failure, since the calling
// function already goes through that crap before we get here.
// Returns: the request number for the process Nymbox request.
// OR returns 0 if the Nymbox was empty (and no message was sent.)
// OR returns -1 if there was an error.
//
// DONE
OT_UTILITY_OT int32_t Utility::sendProcessNymboxLowLevel(
    const string& notaryID, const string& nymID) const // bWasSent is an output
                                                       // param allowing to
                                                       // return whether;
{
    string strLocation = "Utility::sendProcessNymboxLowLevel";

    // Send message..
    OTAPI_Wrap::FlushMessageBuffer();

    int32_t nRequestNum = OTAPI_Wrap::processNymbox(notaryID, nymID);
    if (-1 == nRequestNum) {
        otOut << strLocation
              << ": Failure sending. OT_API_processNymbox() returned -1. \n";
        return -1; // no need to check for any reply.
    }
    if (nRequestNum < 0) {
        otOut << strLocation
              << ": Failure: OT_API_processNymbox() returned unexpected value: "
              << nRequestNum << "\n";
        return -1; // no need to check for any reply.
    }
    if (0 == nRequestNum) {
        otInfo << strLocation << ": Nymbox was empty; no need to process it.\n";
        return 0; // Nymbox is empty, thus no need to process it.
    }

    // Note: I do NOT call RemoveSentMessage for processNymbox, at least, not
    // here.
    // Instead, the place that CALLS this function, will actually use that
    // because
    // it has to be able to harvest the transaction numbers in certain failure
    // cases.

    return nRequestNum;
}

// returns:
// -1 for error,
//  0 for server reply of failure,
//  1 for server reply of success
//
OT_UTILITY_OT int32_t
    Utility::receiveReplySuccessLowLevel(const string& notaryID18,
                                         const string& nymID,
                                         int32_t nRequestNumber7,
                                         const string& IN_FUNCTION)
{
    string strReply = ReceiveReplyLowLevel(
        notaryID18, nymID, nRequestNumber7,
        "receiveReplySuccessLowLevel: " +
            IN_FUNCTION); // <=============== Here we RECEIVE the REPLY...

    // getLastReplyReceived() will also contain the same as strReply.
    // So if the CALLER of this function (that we're in, receiveNymboxLowLevel)
    // wants to see the contents, he can.

    // ReceiveReplyLowLevel returns null unless there was a string returned.
    // So we can directly check it for success...

    return !OTAPI_Wrap::networkFailure() && VerifyMessageSuccess(strReply);
}

// Tries to receive a server reply
// (for a message that you presumably just sent.)
// If successful, returns the server reply. Otherwise returns null.
// (Successful meaning, a valid-formed message was received. Whether that is a
// "success=true" or "success=false" message, the caller will have to figure
// that out for himself.)
//
OT_UTILITY_OT string Utility::ReceiveReplyLowLevel(const string& notaryID17,
                                                   const string& nymID,
                                                   int32_t nRequestNumber8,
                                                   const string& IN_FUNCTION)
{
    setLastReplyReceived("");

    if (0 > nRequestNumber8) {
        otOut << "ReceiveReplyLowLevel (" << IN_FUNCTION
              << "): nRequestNumber isn't a valid number.\n";
        return "";
    }

    string strResponseMessage = OTAPI_Wrap::PopMessageBuffer(
        int64_t(nRequestNumber8), notaryID17, nymID);
    if (!VerifyStringVal(strResponseMessage)) {
        otOut << "ReceiveReplyLowLevel (" << IN_FUNCTION
              << "): no server reply!\n";
        return "";
    }
    setLastReplyReceived(strResponseMessage);

    return strResponseMessage;
}

OT_UTILITY_OT int32_t
    Utility::getRequestNumber(const string& notaryID, const string& nymID)
{
    bool bWasSent = false;
    return getRequestNumber(notaryID, nymID, bWasSent);
}

// -1 == error (couldn't send, or couldn't receive)
// 0 = = success false (received reply from server);
// 1 = = success true  (received reply from server);
//
// To distinguish between error where message wasn't sent,
// and error where message WAS sent, but reply never received,
// bWasSent will be set to TRUE once this function is sure that
// it was sent out. (which you only care about if -1 was the
// return value;
// server reply, AND its status.
// DONE
OT_UTILITY_OT int32_t
    Utility::getRequestNumber(const string& notaryID, const string& nymID,
                              bool& bWasSent) // bWasSent is an output param
                                              // allowing to return whether;
{
    string strLocation = "Utility::getRequestNumber";

    OTAPI_Wrap::FlushMessageBuffer();

    int32_t nResult = OTAPI_Wrap::getRequestNumber(notaryID, nymID);

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": getRequestNumber message failed due to network error.\n";
        return -1;
    }
    if (-1 == nResult) // if error -1, that means it DIDN'T SEND (error)
    {
        otOut << strLocation
        << ": Failed to send getRequestNumber message due to error.\n";
        return -1;
    }
    if (0 == nResult) // if 0 is returned, that also means it DIDN'T SEND (but
                      // there was NO error...)
    { // I don't know if this case can actually even HAPPEN... but if it does,
        // I'll log it.
        otOut << strLocation
              << ": Didn't send this getRequestNumber message, but NO "
                 "error occurred, either. (Should never "
                 "happen.)\n";
        return -1; // Since the 0 case should never happen, I'm returning it as
                   // an ERROR (-1).
                   // Note: I could never return 0;
        // and that the server's reply said "success == 0". But that's not what
        // happened here. Here,
        // we couldn't even SEND our request, which is an error
    }

    //
    // else it's >0  ==  successfully sent! (I BELIEVE this is 1, in this case,
    // every time, since you don't NEED a request number to CALL
    // getRequestNumberNum
    // since you are only calling it in the first place because it must have
    // gotten out of sync.)
    //
    bWasSent = true;
    //
    int32_t nReturn =
        receiveReplySuccessLowLevel(notaryID, nymID, nResult, strLocation);

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": Failed to receiveReplySuccessLowLevel due to network error.\n";
        return -1;
    }

    //      otOut << "IN getRequestNumber " <<
    // getLastReplyReceived());

    // BY this point, we definitely have the request number in nResult, which
    // means
    // the message was actually SENT. (At least.) This also means we can use
    // nResult
    // later to query for a copy of that sent message.
    // Let's go ahead, in this case, and remove that now:
    //
    // var nRemovedGetRequestNumber =
    // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nResult), notaryID,
    // nymID);

    // NOTE: The above call is unnecessary, since a successful reply means
    // we already received the successful server reply, and OT's
    // "ProcessServerReply"
    // already removed the sent message from the sent buffer (so no need to do
    // that here.)

    //      if (nRemovedGetRequestNumber < 1)
    //      {
    //          otOut << "getRequestNumber: ERROR:
    // OT_API_RemoveSentMessage returned: " << nRemovedGetRequestNumber);
    //      }

    return nReturn;
}

// called by getBoxReceiptWithErrorCorrection   DONE
OT_UTILITY_OT bool Utility::getBoxReceiptLowLevel(
    const string& notaryID, const string& nymID, const string& accountID,
    int32_t nBoxType, int64_t strTransactionNum,
    bool& bWasSent) // bWasSent is OTBool
{
    string strLocation = "Utility::getBoxReceiptLowLevel";

    bWasSent = false;

    OTAPI_Wrap::FlushMessageBuffer();

    int32_t nRequestNum = OTAPI_Wrap::getBoxReceipt(
        notaryID, nymID, accountID, nBoxType,
        strTransactionNum); // <===== ATTEMPT TO SEND THE MESSAGE HERE...;

    if (OTAPI_Wrap::networkFailure()) {
        otOut << strLocation
        << ": getBoxReceipt message failed due to network error.\n";
        return false;
    }
    if (-1 == nRequestNum) {
        otOut << strLocation
        << ": Failed to send getBoxReceipt message due to error.\n";
        return false;
    }
    if (0 == nRequestNum) {
        otOut << strLocation << ": Didn't send getBoxReceipt message, but NO error "
                                "occurred, either. (In this case, SHOULD NEVER "
                                "HAPPEN. Treating as Error.)\n";
        return false; // Even though '0' MEANS "didn't send, but no error" by
                      // convention in many places, it is actually an impossible
                      // return value;
    }
    if (nRequestNum < 0) {
        otOut << strLocation << ": Unexpected request number: " << nRequestNum
              << "\n";
        return false;
    }

    bWasSent = true;

    // BY this point, we definitely have the request number, which means the
    // message was actually SENT. (At least.) This also means we can use
    // nRequestNum
    // later to query for a copy of that sent message.
    //
    //
    int32_t nReturn =
        receiveReplySuccessLowLevel(notaryID, nymID, nRequestNum, strLocation);
    otWarn << strLocation << ": nRequestNum: " << nRequestNum
           << " /  nReturn: " << nReturn << "\n";

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": Failed to receiveReplySuccessLowLevel due to network error.\n";
        return -1;
    }

    //     int32_t nRemovedGetBoxReceipt =
    // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nRequestNum), notaryID,
    // nymID);
    //
    //      // NOTE: The above call is unnecessary, since a successful reply
    // means
    //      // we already received the successful server reply, and OT's
    // "ProcessServerReply"
    //      // already removed the sent message from the sent buffer (so no need
    // to do that here.)
    //
    //      if (nRemovedGetBoxReceipt < 1)
    //      {
    //          otOut << "getBoxReceiptLowLevel: ERROR:
    // OT_API_RemoveSentMessage returned: " << nRemovedGetBoxReceipt);
    //      }

    if (nReturn > 0) {
        return true;
    }

    otOut << strLocation << ": Failure: Response from server:\n"
          << getLastReplyReceived() << "\n";

    return false;
}

// called by insureHaveAllBoxReceipts     DONE
OT_UTILITY_OT bool Utility::getBoxReceiptWithErrorCorrection(
    const string& notaryID, const string& nymID, const string& accountID,
    int32_t nBoxType, int64_t strTransactionNum) // nBoxType is int32_t
{
    string strLocation = "Utility::getBoxReceiptWithErrorCorrection";

    bool bWasSent = false;
    bool bWasRequestSent = false;

    if (getBoxReceiptLowLevel(notaryID, nymID, accountID, nBoxType,
                              strTransactionNum, bWasSent)) {
        return true;
    }
    if (bWasSent && (1 == getRequestNumber(notaryID, nymID,
                                           bWasRequestSent))) // this might be
                                                              // out of sync, if
                                                              // it failed...
                                                              // we'll re-sync,
                                                              // and re-try.
    {
        if (bWasRequestSent &&
            getBoxReceiptLowLevel(notaryID, nymID, accountID, nBoxType,
                                  strTransactionNum, bWasSent)) {
            return true;
        }
        otOut << strLocation << ": getBoxReceiptLowLevel failed, then "
                                "getRequestNumber succeeded, then "
                                "getBoxReceiptLowLevel failed again. (I give "
                                "up.)\n";
    }
    else {
        otOut << strLocation
              << ": getBoxReceiptLowLevel failed, then "
                 "getRequestNumber failed. (I give up.) Was "
                 "getRequestNumber message sent: " << bWasRequestSent << "\n";
    }
    return false;
}

// This function assumes you just downloaded the latest version of the box
// (inbox, outbox, or nymbox)
// and its job is to make sure all the related box receipts are downloaded as
// well and available, though
// not necessarily loaded into memory. (Yet.)
// NOTE: If nBoxType is 0 (nymbox) then pass nymID as the accountID as well!
//
OT_UTILITY_OT bool Utility::insureHaveAllBoxReceipts(
    const string& notaryID, const string& nymID, const string& accountID,
    int32_t nBoxType) // nBoxType is int32_t
{
    bool bFoundIt = false;
    int32_t nRequestSeeking = 0;
    return insureHaveAllBoxReceipts(notaryID, nymID, accountID, nBoxType,
                                    nRequestSeeking, bFoundIt);
}

OT_UTILITY_OT bool Utility::insureHaveAllBoxReceipts(
    const string& notaryID, const string& nymID, const string& accountID,
    int32_t nBoxType, int32_t nRequestSeeking, bool& bFoundIt)
{
    string strLocation = "Utility::insureHaveAllBoxReceipts";

    string ledger = "";

    if (0 == nBoxType) {
        ledger = OTAPI_Wrap::LoadNymboxNoVerify(notaryID, nymID);
    }
    else if (1 == nBoxType) {
        ledger = OTAPI_Wrap::LoadInboxNoVerify(notaryID, nymID, accountID);
    }
    else if (2 == nBoxType) {
        ledger = OTAPI_Wrap::LoadOutboxNoVerify(notaryID, nymID, accountID);
    }
    else {
        otOut << strLocation << ": Error. Expected nBoxType of 0,1,2 (nymbox, "
                                "inbox, or outbox.)\n";
        return false;
    }

    // Unable to load or verify inbox/outbox/nymbox
    // Notice I don't call VerifyAccount() here (not that the API even exposes
    // that method) but why not? Because that method tries to load up all the
    // box receipts, in addition to verifying the signature. Therefore I call
    // "Load XXXX NoVerify()", avoiding all that, then I verify the Signature
    // itself. That's because this function's whole point is to find out what
    // the box receipts are, and download them from the server. No point trying
    // to load them before that time, when I know it will fail.
    //
    if (!VerifyStringVal(ledger) ||
        (!OTAPI_Wrap::VerifySignature(nymID, ledger))) {
        otOut << strLocation << ": Unable to load or verify signature on "
                                "ledger. (Failure.) Contents:\n" << ledger
              << "\n";
        return false;
    }

    // At this point, the box is definitely loaded.
    // Next we'll iterate the receipts
    // within, and for each, verify that the Box Receipt already exists. If not,
    // then we'll download it using getBoxReceiptLowLevel(). If any download
    // fails,
    // then we break out of the loop (without continuing on to try the rest.)
    //
    bool bReturnValue = true; // Assuming an empty box, we return success;

    int32_t nReceiptCount =
        OTAPI_Wrap::Ledger_GetCount(notaryID, nymID, accountID, ledger);
    if (nReceiptCount > 0) {
        for (int32_t i_loop = 0; i_loop < nReceiptCount; ++i_loop) {
            int64_t lTransactionNum =
                OTAPI_Wrap::Ledger_GetTransactionIDByIndex(
                    notaryID, nymID, accountID, ledger, i_loop);
            if (lTransactionNum != -1) {
                if (lTransactionNum > 0) {
                    string strTransaction =
                        OTAPI_Wrap::Ledger_GetTransactionByID(notaryID, nymID,
                                                              accountID, ledger,
                                                              lTransactionNum);

                    // Note: OTAPI_Wrap::Ledger_GetTransactionByID tries to get
                    // the full transaction from the ledger and
                    // return it;
                    // and pass the full version back to us. Failing that
                    // (perhaps the box receipt hasn't been downloaded
                    // yet) then it will try to re-sign / re-save the
                    // abbreviated version and pass it back. So if, by this
                    // point, it STILL has failed, that means it couldn't even
                    // give us the abbreviated version either!
                    //
                    if (!VerifyStringVal(strTransaction)) {
                        otOut << strLocation
                              << ": Error: Null transaction somehow returned, "
                                 "(not even an abbreviated one!) even though I "
                                 "had a good ID " << lTransactionNum
                              << " which came originally as lTransactionNum: "
                              << lTransactionNum << " for index: " << i_loop
                              << " with the contents:\n\n" << strTransaction
                              << "\n\n";
                        return false;
                    }

                    // This block might have a full version, OR an abbreviated
                    // version of the box receipt in
                    // question, in the strTransaction variable. It will attempt
                    // to download the full box
                    // receipt, if we don't already have it here on the client
                    // side.
                    {
                        string strTransType = OTAPI_Wrap::Transaction_GetType(
                            notaryID, nymID, accountID, strTransaction);
                        bool bIsReplyNotice = (VerifyStringVal(strTransType) &&
                                               (strTransType == "replyNotice"));
                        int64_t lRequestNum = 0;
                        if (bIsReplyNotice) {
                            lRequestNum = OTAPI_Wrap::ReplyNotice_GetRequestNum(
                                notaryID, nymID, strTransaction);
                        }

                        bool bShouldDownload =
                            (!bIsReplyNotice ||
                             (bIsReplyNotice && (0 < lRequestNum) &&
                              !OTAPI_Wrap::HaveAlreadySeenReply(notaryID, nymID,
                                                                lRequestNum)));

                        if (bShouldDownload) // This block executes if we should
                                             // download it (assuming we haven't
                                             // already, which it also checks
                                             // for.)
                        {
                            bool bHaveBoxReceipt =
                                OTAPI_Wrap::DoesBoxReceiptExist(
                                    notaryID, nymID, accountID, nBoxType,
                                    lTransactionNum);
                            if (!bHaveBoxReceipt) {
                                otWarn << strLocation << ": Downloading box "
                                                         "receipt to add to my "
                                                         "collection...\n";

                                bool bDownloaded =
                                    getBoxReceiptWithErrorCorrection(
                                        notaryID, nymID, accountID, nBoxType,
                                        lTransactionNum);
                                if (!bDownloaded) {
                                    otOut
                                        << strLocation
                                        << ": Failed downloading box receipt. "
                                           "(Skipping any others.) Transaction "
                                           "number: " << lTransactionNum
                                        << "\n";

                                    bReturnValue = false;
                                    break;
                                    // No point continuing to loop and fail 500
                                    // times, when
                                    // getBoxReceiptWithErrorCorrection()
                                    // already failed
                                    // even doing the getRequestNumber() trick
                                    // and
                                    // everything, and whatever retries are
                                    // inside OT, before it finally
                                    // gave up.
                                }
                                // else (Download success.)
                            } // if (!bHaveBoxReceipt)
                        }

                        // else we already have the box receipt, no need to
                        // download again.
                    }
                } // if (lTransactionNum > 0)
                else {
                    otOut
                        << strLocation
                        << ": Error: TransactionNum less-than-or-equal-to 0.\n";
                }
            }
            else {
                otOut << strLocation << ": Error: TransactionNum was null, "
                                        "when trying to read it based on the "
                                        "index (within bounds, too!)\n";
            }
        } // ************* FOR LOOP ******************
    }     // if (nReceiptCount > 0)

    //
    // if nRequestSeeking is >0, that means the caller wants to know if there is
    // a receipt present for that request number.
    // (which is only a valid option ifnBoxType = = 0 for Nymbox.);
    // IF the receipt is found, then bFoundIt is set to true.
    //
    if ((nRequestSeeking > 0) && (0 == nBoxType)) {
        // NOTE: the below call to OTAPI_Wrap::Nymbox_GetReplyNotice will
        // succeed even if
        // only the abbreviated receipt is available, because the caller mainly
        // just
        // wants to know if it is there.
        // Technically the full receipt SHOULD always be there, with the above
        // loop,
        // but since the above loop can break in case of error, it's still
        // possible that
        // box receipts haven't been downloaded by the time you reach this code.
        // Nevertheless, we will see if the reply is there for the appropriate
        // request
        // number, whether abbreviated or not.
        //
        // UPDATE: I am now adding specific cases where the replyNotice is NOT
        // downloaded.
        // You still use it, through its abbreviated version -- and the actual
        // version
        // IS still available for download through the server's API. But with a
        // replyNotice,
        // just knowing that it exists is usually enough for the client, who
        // probably still
        // has a cached copy of the original sent message anyway. Only in cases
        // where he
        // doesn't, would he need to download it. (Why? So he can process the
        // server's reply.)
        // Therefore the cached sent message is useless, since it doesn't
        // contain the server's
        // reply! Hmm. So I need that reply BUT ONLY IN CASES where I didn't
        // already receive it
        // as a normal part of processing (and that is MOST of the time, meaning
        // most cases can
        // thus entirely eliminate the download.)
        //
        // PROTOCOL FOR NOT DOWNLOADING MOST OF THE BOX RECEIPTS
        //
        // Solution: User messages should contain a list of the last X number of
        // request numbers
        // that they have DEFINITELY seen the response to. The server,
        // meanwhile, since the user
        // has DEFINITELY seen the response, can now safely remove the
        // replyNotice from the Nymbox.
        // The only reason it was there in the first place was to make sure the
        // user got the reply.
        // Since the user is straight-up acknowledging that he received it, the
        // server no longer
        // needs to "make sure" and thus it can remove that reply from the
        // Nymbox, and mark the
        // box receipt for deletion. This will be MOST REPLIES! We'll eliminate
        // the step of having
        // to download the box receipt.
        // The above call to getBoxReceiptWithErrorCorrection should also be
        // smart enough not to
        // bother downloading any replyNotice Box Receipts if their request
        // number appears on that
        // list. Again: the list means I DEFINITELY already responded to it--if
        // the request # is on
        // that list, then NO NEED downloading the Box Receipt -- I DEFINITELY
        // already got that reply!
        //
        // Therefore, Something like OTAPI_Wrap::HaveAlreadySeenReply(notaryID,
        // nymID, requestNum);
        //
        // Perhaps also, on the server side, send a list of request numbers for
        // that Nym that the
        // server KNOWS the Nym saw the reply to. This way, the Nym can remove
        // the number from his
        // list, and thus won't be continually causing the server to load up the
        // Nymbox and try
        // to remove the replyNotice (since it's already been removed.)
        //
        // The Nym doesn't have to keep a list of ALL request numbers he's seen
        // the reply to.
        // Rather, just the past X number of them, and with the number
        // explicitly removed once
        // he sees the server acknowledgment. (ServerAckOfAlreadyGotReply.)
        //
        // The server, meanwhile, is free to remove the ACK for any request #
        // once he sees that
        // the client has as well. Server also only needs to store a list of the
        // past X request #s.
        // Also: since both sides REMOVE the number, there need not necessarily
        // be a limit on the
        // size of the list, since it grows and shrinks as needed.
        //
        // Whenever Wallet processes a server reply, just see if it's on that
        // "replied to already"
        // list already on client side. If so, discard the reply.
        // OTClient::ProcessServerReply probably
        // best place to do  (We replied to it already, so discard it.)
        // Also, for any server reply, look at the list of numbers on it. The
        // server is acknowledging
        // to us that it KNOWS we got those replies, and that it ALREADY has
        // removed them from the
        // Nymbox as a result. Therefore we can remove ALL of those numbers from
        // our own list
        // as well. No need for an API call to do this, since it will happen
        // internally to OT.
        //
        // On the server side, any numbers on its own list were only there to
        // acknowledge numbers
        // that had been on the client side list. Therefore, when those numbers
        // disappear from the
        // client side list, the server simply removes them. Again: ANY NUMBERS
        // on the server list,
        // which do NOT appear on the client list, are REMOVED From the server
        // list. After all, the
        // client has clearly now removed them, so the server doesn't have to
        // keep them around either.
        //
        // These are useful for synchronization but also there's a int64_t term
        // benefit, if we include
        // them in the signed receipt (which they will be already, if the
        // receipt contains the entire
        // message and not just the transaction.) That benefit is that we can
        // prove receipt of notice.
        // At least, notice of server replies. But for other notice types, such
        // as notice of upcoming
        // meeting. Or notice of upcoming election. Or notice of election
        // outcome. Or notice of petition
        // to put X issue on the next ballot, or to nominate Y Nym for some
        // corporate role. Sometimes
        // you want to be able to PROVE that notice was received. Does this
        // prove that?
        // Hmm, not necessarily. Currently I'm using this as an optimization
        // scheme, which is useful
        // even if not provable. How to make it provable?
        //
        // Back from tangent: Wait a sec! If I notice the server that I saw the
        // reply, the server will
        // remove that reply from my Nymbox -- but it's still in my Nymbox on
        // the client side! Until
        // I download the latest Nymbox. Thus if I try to PROCESS MY NYMBOX, I
        // will be attempting to
        // accept a receipt that's already gone! (And the processNymbox will
        // therefore FAIL!)
        // Solution: be smart enough, when processing Nymbox, to IGNORE any
        // replyNotices when the request
        // Number appears on the client's list! As the wallet processes the
        // Nymbox it should already
        // know to skip the ones that were already replied-to.
        // Meanwhile the server side will deliberately NOT update the Nymbox
        // hash just because the receipt
        // was removed. Otherwise it could trigger an unnecessary download of
        // the Nymbox, when the whole
        // point of this exercise was to prevent unnecessary downloads. It only
        // updates the Nymbox hash
        // when it WANTS me to download the Nymbox, and that certainly does NOT
        // apply to cases where the
        // only change involved the removal of some old receipt I already
        // acknowledged. (No need to force
        // any downloads based on THAT case, after all.)
        //
        string strReplyNotice = OTAPI_Wrap::Nymbox_GetReplyNotice(
            notaryID, nymID, int64_t(nRequestSeeking));

        if (VerifyStringVal(strReplyNotice)) {
            bFoundIt = true;
        }
    }

    return bReturnValue;
}

/*
static void getBoxReceipt( string NOTARY_ID, string NYM_ID, string ACCT_ID, //
If for Nymbox (vs inbox/outbox) then pass NYM_ID
in this field also.
int32_t  nBoxType, // 0/nymbox, 1/inbox, 2/outbox
const string TRANSACTION_NUMBER);

static bool DoesBoxReceiptExist( string NOTARY_ID, string NYM_ID, string
ACCT_ID, // If for Nymbox (vs inbox/outbox) then pass NYM_ID
in this field also.
int32_t  nBoxType, // 0/nymbox, 1/inbox, 2/outbox
const string TRANSACTION_NUMBER);
*/
// If the transaction number requests fail, this function will try to resync
// the request number. But you still have to call getRequestNumber() yourself if
// you have a failure in your own function, since you might already have
// enough transaction numbers, and thus this function will never get called,
// even if your request number IS out of sync. Sorry :-)
//

OT_UTILITY_OT int32_t
    Utility::getTransactionNumLowLevel(const string& notaryID,
                                       const string& nymID,
                                       bool& bWasSent) // bWasSent is OTBool
{
    string strLocation = "Utility::getTransactionNumLowLevel";

    OTAPI_Wrap::FlushMessageBuffer();
    bWasSent = false;

    int32_t nRequestNum = OTAPI_Wrap::getTransactionNumbers(
        notaryID, nymID); // <===== ATTEMPT TO SEND THE MESSAGE HERE...;

    if (OTAPI_Wrap::networkFailure()) {
        otOut << strLocation
        << ": getNymbox message failed due to network error.\n";
        return -1;
    }
    if (-1 == nRequestNum) {
        otOut << strLocation
        << ": Failed to send getNymbox message due to error.\n";
        return -1;
    }
    if (0 == nRequestNum) {
        otOut << strLocation << ": Unexpectedly returned 0. Didn't send "
                                "getTransactionNumbers message, but NO error "
                                "occurred, either. (In this case, SHOULD NEVER "
                                "HAPPEN. Treating as Error.)\n";
        return -1; // Even though '0' MEANS "didn't send, but no error" by
                   // convention in many places, it is actually an impossible
                   // return value;
    }
    if (nRequestNum < 0) {
        otOut << strLocation << ": Unexpected request number: " << nRequestNum
              << "\n";
        return -1;
    }

    bWasSent = true;

    //
    int32_t nReturn = receiveReplySuccessLowLevel(notaryID, nymID, nRequestNum,
                                                  "getTransactionNumbers");

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": Failed to receiveReplySuccessLowLevel due to network error.\n";
        return -1;
    }

    //      otOut << "IN getTransactionNumbers " <<
    // getLastReplyReceived());

    // BY this point, we definitely have the request number in nResult, which
    // means
    // the message was actually SENT. (At least.) This also means we can use
    // nResult
    // later to query for a copy of that sent message.
    // Let's go ahead, in this case, and remove that now:
    //

    // THE REMOVE SENT MESSAGE BELOW FAILS, LIKE IT'S ALREADY GONE.
    //
    // THIS MUST BE DUE TO THE PROCESS SERVER REPLY THAT OCCURS **IMMEDIATELY**
    // after the message was originally sent!
    // (The reply came in and was sent to OT's "ProcessServerReply", INSIDE the
    // call to OTAPI_Wrap::getTransactionNumber.)
    // Our subsequent "receive" (above) is nothing of the sort, but actually
    // pops the incoming message buffer where
    // the server's reply was ALREADY SITTING, since it was put there in OT's
    // "ProcessServerReply", WHICH REMOVED THE
    // SENT MESSAGE ALREADY (that's why the below call to RemoveSentMessage
    // fails.)
    //
    // RETHINK any logic that doesn't take this into account,.
    // Either we REMOVE this call wherever this happens, OR... we call Get first
    // and make sure whether it's
    // there, THEN remove it. But we can't be lumping "Failure because it's
    // gone" versus "Error state" by mixing
    // 0 and -1 here. We need to differentiate.
    //
    // Bottom line: if the reply WAS received, then the original sent message
    // has ALREADY been removed
    // from the sent buffer. Whereas if the reply was NOT received, then the
    // sent message is still there,
    // but in that case, we do NOT want to remove it -- we want it to STAY in
    // the sent buffer, so that
    // when we get the Nymbox later and we DO have the reply from that, THEN we
    // can remove the sent msg from
    // the sent buffer. Until then, we don't want OT to think it's already been
    // processed (which it will, if
    // it's already been removed from the sent buffer. So we leave it there for
    // now.)
    //
    //
    //
    //     int32_t nRemovedSentMsg =
    // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nRequestNum), notaryID,
    // nymID);
    //
    //      if (nRemovedSentMsg < 1)
    //      {
    //          otOut << "getTransactionNumbers: ERROR:
    // OT_API_RemoveSentMessage returned: " << nRemovedSentMsg);
    //      }

    if (1 == nReturn) {
        return nRequestNum;
    }

    return nReturn;
}

// DONE
OT_UTILITY_OT bool Utility::getTransactionNumbers(const string& notaryID,
                                                  const string& nymID)
{
    return getTransactionNumbers(notaryID, nymID, true); // bForceFirstCall = =
                                                         // true (by default)
                                                         // but in special cases
                                                         // you can override it
                                                         // and set it to false.
}

OT_UTILITY_OT bool Utility::getTransactionNumbers(
    const string& notaryID, const string& nymID,
    bool bForceFirstCall) // boolean bForceFirstCall defaults to true.
{
    string strLocation = "Utility::getTransactionNumbers";

    bool bWasSent = false;
    int32_t nGetNumbers = -1;
    if (bForceFirstCall) {
        nGetNumbers = getTransactionNumLowLevel(
            notaryID, nymID, bWasSent); // <============ FIRST TRY;
    }

    // if the first call didn't happen, due to bForceFirstCall being false, that
    // means the caller wants the rest of this to happen as though it did.
    //
    if (!bForceFirstCall || (bWasSent && (nGetNumbers >= 1)) ||
        (!bWasSent && (nGetNumbers == 0))) {
        // Because it was successful, we have to now SIGN FOR those numbers we
        // requested.
        //
        bool bForceDownload = true;
        int32_t nProcess = getAndProcessNymbox_4(
            notaryID, nymID, bWasSent, bForceDownload); // bForceDownload=true;

        if ((bWasSent && (1 == nProcess)) || (!bWasSent && (0 == nProcess))) {
            return true;
        }
    }
    // If value is LESS THAN -1 (which is an unexpected value)
    // or if the getTransactionNumbers message WASN'T EVEN SENT, then return.
    //
    else if ((nGetNumbers < -1) || !bWasSent) {
        otOut << strLocation << ": Failure: getTransactionNumLowLevel returned "
                                "unexpected value: " << nGetNumbers << "\n";
        return false;
    }

    // Below this point, the message WAS sent.  -1 is error, 0 is failure, >0 is
    // success.
    // Now it's just about whether a reply was successful, or was even received.
    //
    //      else if ((-1 == nGetNumbers) ||   // Message sent, but then error
    // receiving or loading the reply.
    //               (( 0) == nGetNumbers))     // Received a reply, butstatus =
    // = failure on that reply.;
    else if ((-1 == nGetNumbers) || (0 == nGetNumbers)) {
        if (-1 == nGetNumbers) {
            otOut << strLocation << ": FYI: getTransactionNumLowLevel did "
                                    "send, but returned error (-1). "
                                    "(Re-trying...)\n";
        }
        else if (0 == nGetNumbers) {
            otOut << strLocation << ": FYI: getTransactionNumLowLevel did "
                                    "send, but returned failure (0). "
                                    "(Re-trying...)\n";
        }

        int32_t nGetRequestNumber = getRequestNumber(notaryID, nymID);
        if (1 != nGetRequestNumber) {
            otOut << strLocation << ": Failure: getTransactionNumLowLevel "
                                    "failed, then I tried to resync with "
                                    "getRequestNumber and then that failed "
                                    "too. (I give up.)\n";
            return false;
        }

        bool bWasProcessSent = false;
        int32_t nProcessNymbox =
            getAndProcessNymbox_4(notaryID, nymID, bWasProcessSent, true);

        //          if ( (!bWasProcessSent && ((nProcessNymbox  < 0) ||
        // (nProcessNymbox  > 1))) ||
        //               ( bWasProcessSent && (nProcessNymbox  != 1)) ) // -1
        // error, 0 failed (harvesting success), 1 success, >1 failed
        // (harvesting NOT done) RequestNum is returned.

        if ((!bWasProcessSent &&
             ((nProcessNymbox < 0) || (nProcessNymbox > 1))) ||
            (bWasProcessSent && (nProcessNymbox != 1))) {
            // todo: if request num is returned probably don't have to do
            // anything with it.
            // Why not?  Because future processNymbox will iterate Nymbox and
            // search for all found
            // items in the sent message buffer, and REMOVE them from it (as
            // clearly they will be
            // processed already.)
            // The ones left over in the sent buffer, after this? Must be
            // harvested!
            // Hmm, solution: Use the "Flush Sent Messages" function, which is
            // already
            // there. Have it be smart enough to harvest all sent messages
            // before flushing,
            //
            //
            if (bWasProcessSent && (nProcessNymbox > 1)) {
                string strNymbox = OTAPI_Wrap::LoadNymboxNoVerify(
                    notaryID,
                    nymID); // FLUSH SENT MESSAGES!!!!  (AND HARVEST.);

                if (VerifyStringVal(strNymbox)) {
                    OTAPI_Wrap::FlushSentMessages(
                        false, // harvesting for retry = = OT_FALSE
                        notaryID, nymID, strNymbox);
                }
            }

            otOut << strLocation
                  << ": Failure: getAndProcessNymbox. Returned value: "
                  << nProcessNymbox << "\n";

            return false;
        }

        nGetNumbers = getTransactionNumLowLevel(
            notaryID, nymID, bWasSent); // <================= SECOND TRY;

        //          if ( ( bWasSent && (nGetNumbers >=  1)) || // if message was
        // sent, and was a success.
        //               (!bWasSent && (nGetNumbers ==  0)) )  // Or if message
        // wasn't sent due to "you already signed out too many numbers--you need
        // to process your Nymbox..."

        if ((bWasSent && (nGetNumbers >= 1)) ||
            (!bWasSent && (nGetNumbers == 0))) {
            int32_t nProcess =
                getAndProcessNymbox_4(notaryID, nymID, bWasSent, true);

            //              if ( ( bWasSent && (1 == nProcess)) ||
            //                   (!bWasSent && (0 == nProcess)) )

            if ((bWasSent && (1 == nProcess)) ||
                (!bWasSent && (0 == nProcess))) {
                return true;
            }
        }
        //          else if (( nGetNumbers < -1) ||
        //                   (!bWasSent && nGetNumbers != 0))
        else if ((nGetNumbers < -1) || (!bWasSent && nGetNumbers != 0)) {
            otOut << strLocation << ": Failure: getTransactionNumLowLevel "
                                    "returned unexpected value: " << nGetNumbers
                  << "\n";
            return false;
        }
        else if ((-1 == nGetNumbers) || (0 == nGetNumbers)) {
            if (-1 == nGetNumbers) {
                otOut << strLocation << ": Failure: getTransactionNumLowLevel "
                                        "did send, but returned error (-1), "
                                        "even after syncing the request number "
                                        "successfully. (Giving up.)\n";
            }
            else if (0 == nGetNumbers) {
                otOut << strLocation << ": Failure: getTransactionNumLowLevel "
                                        "did send, but returned failure (0), "
                                        "even after syncing the request number "
                                        "successfully. (Giving up.)\n";
            }

            bool forceDownload = true;

            int32_t nLast = getAndProcessNymbox_4(
                notaryID, nymID, bWasProcessSent, forceDownload);
            //              if (
            //                  ((!bWasProcessSent) && ((nLast  < 0) || (nLast
            // > 1))) ||
            //                  ((true  == bWasProcessSent) &&  (nLast != 1))
            //                  ) // -1 error, 0 failed (harvesting success), 1
            // success, >1 failed (harvesting NOT done) RequestNum is returned.

            if (((!bWasProcessSent) && ((nLast < 0) || (nLast > 1))) ||
                ((true == bWasProcessSent) &&
                 (nLast != 1))) // -1 error, 0 failed (harvesting success), 1
                                // success, >1 failed (harvesting NOT done)
                                // RequestNum is returned.
            {
                if (bWasProcessSent && (nLast > 1)) {
                    string strNymbox = OTAPI_Wrap::LoadNymboxNoVerify(
                        notaryID,
                        nymID); // FLUSH SENT MESSAGES!!!!  (AND HARVEST.);

                    if (VerifyStringVal(strNymbox)) {
                        OTAPI_Wrap::FlushSentMessages(
                            false, // harvesting for retry = = OT_FALSE
                            notaryID, nymID, strNymbox);
                    }
                }

                otOut << strLocation
                      << ": Failure: getAndProcessNymbox. Returned value: "
                      << nLast << "\n";
                return false;
            }

            nGetNumbers = getTransactionNumLowLevel(
                notaryID, nymID, bWasSent); // <============ FIRST TRY;

            //              if ( (bWasSent && (nGetNumbers >= 1) )
            //                   ||
            //                   ((!bWasSent && (nGetNumbers == 0) ) )
            //                 )

            if ((bWasSent && (nGetNumbers >= 1)) ||
                ((!bWasSent && (nGetNumbers == 0)))) {
                int32_t nProcess =
                    getAndProcessNymbox_4(notaryID, nymID, bWasSent, true);

                //                  if ( ( bWasSent && (1 == nProcess)) ||
                //                       (!bWasSent && (0 == nProcess)) )

                if ((bWasSent && (1 == nProcess)) ||
                    (!bWasSent && (0 == nProcess))) {
                    return true;
                }
            }

            //              if ((nGetNumbers < -1) ||         // If value is
            // LESS THAN -1 (which is an unexpected value)
            //                  !bWasSent)    // or if the getTransactionNumbers
            // message WASN'T EVEN SENT, then return.

            if ((nGetNumbers < -1) || !bWasSent) {
                otOut << strLocation
                      << ": Failure: getTransactionNumLowLevel returned "
                         "unexpected value: " << nGetNumbers << "\n";
                return false;
            }
        }
    }

    // BY THIS POINT, we have SUCCESSFULLY sent the getTransactionNumLowLevel
    // message,
    // and nGetNumbers contains its request number.

    // No need to read the result, as getTransactionNumLowLevel() already read
    // it,
    // and and it's available anytime via getLastReplyReceived()

    // todo: should the member variable strLastReplyReceived be set to this
    // one?
    // before, the member var was shadowed (string strLastReplyReceived =
    // getLastReplyReceived();).
    string lastReplyReceived = getLastReplyReceived();

    if (!VerifyStringVal(lastReplyReceived)) {
        otOut << strLocation << ": ERROR in getLastReplyReceived(): why was "
                                "this string not set, when getRequestNumber "
                                "was otherwise an apparent success?\n";
        return false; // (SHOULD NEVER HAPPEN. This string is set in the
                      // getRequestNumber function.);
    }

    // BY THIS POINT, we have received a server reply: getTransactionNumResponse
    // (Unless it is malformed.) It's definitely not null, nor empty.

    // Grab the NymboxHash on the getTransactionNumResponse reply, and also the
    // one I
    // already had on my client-side Nym... (So we can compare them.)
    //
    string strServerHash = OTAPI_Wrap::Message_GetNymboxHash(lastReplyReceived);
    bool bServerhash = VerifyStringVal(strServerHash);
    if (!bServerhash) {
        otOut << strLocation
              << ": Warning: Unable to retrieve server-side NymboxHash from "
                 "OT, from server getTransactionNumResponse reply:\n\n"
              << strLastReplyReceived << "\n";
        //          return false;
    }

    string strLocalHash = OTAPI_Wrap::GetNym_NymboxHash(notaryID, nymID);
    bool bLocalhash = VerifyStringVal(strLocalHash);
    if (!bLocalhash) {
        otOut << strLocation
              << ": Warning: Unable to retrieve client-side NymboxHash from "
                 "OT, for:\n notaryID: " << notaryID << "\n nymID: " << nymID
              << "\n";
        //          return false;
    }

    // the hashes don't match -- so let's definitely re-try to download the
    // latest nymbox.
    //
    if (!bServerhash || !bLocalhash ||
        (bServerhash && bLocalhash && !(strServerHash == strLocalHash))) {
        // the getRequestNumber worked, and the server hashes don't match,
        // so let's get and process the Nymbox...
        //

        bool bWasProcessSent = false;
        int32_t nGetNymbox =
            getAndProcessNymbox_4(notaryID, nymID, bWasProcessSent, true);

        //          if ( ((!bWasProcessSent) && ((nGetNymbox  < 0) ||
        // (nGetNymbox  > 1))) ||
        //               ((true  == bWasProcessSent) && (nGetNymbox != 1)) ) //
        // -1 error, 0 failed (harvesting success), 1 success, >1 failed
        // (harvesting NOT done) RequestNum is returned.

        if (((!bWasProcessSent) && ((nGetNymbox < 0) || (nGetNymbox > 1))) ||
            ((true == bWasProcessSent) && (nGetNymbox != 1))) {
            if (nGetNymbox > 1) {
                string strNymbox = OTAPI_Wrap::LoadNymboxNoVerify(
                    notaryID,
                    nymID); // FLUSH SENT MESSAGES!!!!  (AND HARVEST.);

                if (VerifyStringVal(strNymbox)) {
                    OTAPI_Wrap::FlushSentMessages(
                        false, // harvesting for retry = = OT_FALSE
                        notaryID, nymID, strNymbox);
                }
            }

            otOut
                << strLocation
                << ": Failure: getAndProcessNymbox returned unexpected value: "
                << nGetNymbox << "\n";
            return false;
        }
        if (-1 == nGetNymbox) // we'll try re-syncing the request number, then
                              // try again.
        {
            otOut << strLocation << ": Failure: getAndProcessNymbox returned "
                                    "-1, even after syncing the request number "
                                    "successfully. (Giving up.)\n";
            return false;
        }
    }

    return true;
}

OT_UTILITY_OT bool Utility::getIntermediaryFiles(const string& notaryID,
                                                 const string& nymID,
                                                 const string& accountID)
{
    bool bForceDownload = false;
    return getIntermediaryFiles(notaryID, nymID, accountID, bForceDownload);
}

// NOTE: This is a new version, that uses getInboxAccount new version, which
// uses getAccountData instead of getAccount, getInbox, and getOutbox.
OT_UTILITY_OT bool Utility::getIntermediaryFiles(
    const string& notaryID, const string& nymID, const string& accountID,
    bool bForceDownload) // booleanbForceDownload = false;
{
    string strLocation = "Utility::getIntermediaryFiles";

    if (!VerifyStringVal(notaryID) || notaryID.size() < 10) {
        otOut << strLocation << ": nullptr or invalid notaryID.\n";
        return false;
    }
    if (!VerifyStringVal(nymID) || nymID.size() < 10) {
        otOut << strLocation << ": nullptr or invalid nymID.\n";
        return false;
    }
    if (!VerifyStringVal(accountID) || accountID.size() < 10) {
        otOut << strLocation << ": nullptr or invalid accountID.\n";
        return false;
    }

    bool bWasSentInbox = false;
    bool bWasSentAccount = false;

    // This is a new version of getInboxAccount that downloads ALL
    // THREE files (account/inbox/outbox) in a single server message.
    //
    int32_t nGetInboxAcct =
        getInboxAccount(notaryID, nymID, accountID, bWasSentInbox,
                        bWasSentAccount, bForceDownload);

    // if we received an error state, and the "getAccountData" message wasn't
    // even sent,
    // then no point doing a bunch of retries -- it failed.
    //
    if (-1 == nGetInboxAcct) {
        if (!bWasSentAccount) {
            otOut << strLocation << ": this.getInboxAccount failed, without "
                                    "even sending getAccountData. (Returning "
                                    "false.)\n";
            return false;
        }
    }

    // If it wasn't sent, and 0 was returned, that means
    // no error: we already have the latest inbox. (Nothing done.)
    //
    else if (!bWasSentInbox && (0 == nGetInboxAcct)) {
        // we don't return true;
    }
    else if (1 != nGetInboxAcct) {
        otOut << strLocation
              << ": getInboxAccount failed. (Trying one more time...)\n";

        int32_t nGetRequestNumber = getRequestNumber(notaryID, nymID);
        if (1 != nGetRequestNumber) {
            otOut << strLocation << ": Failure: getInboxAccount failed, then I "
                                    "tried to resync with getRequestNumber and "
                                    "then that failed too. (I give up.)\n";
            return false;
        }

        ///////////////////////// WARNING: DEAD CODE
        ///?????????????????????????????????

        // We sync'd the request number, so now we try the function again...
        //
        int32_t nSecondtry =
            getInboxAccount(notaryID, nymID, accountID, bWasSentInbox,
                            bWasSentAccount, bForceDownload);

        if ((-1 == nSecondtry) && !bWasSentAccount) {
            // if we received an error state, and the "getAccountData" message
            // wasn't even sent,
            // then no point doing a bunch of retries -- it failed.
            //
            otOut << strLocation << ": getInboxAccount failed a second time, "
                                    "without even sending getAccountData. "
                                    "(Returning false.)\n";
            return false;
        }
        // If it wasn't sent, and 0 was returned, that means
        // no error: we already have the latest inbox. (Nothing done.)
        //
        if (!bWasSentInbox && (0 == nSecondtry)) {
            // we don't return true;
        }
        else if (1 != nSecondtry) {
            otOut << strLocation
                  << ": getInboxAccount re-try failed. (That's twice "
                     "now--Returning false.) Value: " << nSecondtry << "\n";
            return false;
        }
        otOut << strLocation
              << ": getInboxAccount second call succeeded. (Continuing...)\n";
    }

    return true;
}

// NOTE: This is a new version that uses the new server message, getAccountData
// (Which combines getAccount, getInbox, and getOutbox into a single message.)
OT_UTILITY_OT int32_t
    Utility::getInboxAccount(const string& notaryID, const string& nymID,
                             const string& accountID, bool& bWasSentInbox,
                             bool& bWasSentAccount, const bool)
{
    string strLocation = "Utility::getInboxAccount";

    bWasSentAccount = false;
    bWasSentInbox = false;

    //
    // (Success means both were downloaded, if necessary.)
    //
    // FIRST WE DO THE ACCOUNT...
    //
    // GET ACCOUNT
    //
    OTAPI_Wrap::FlushMessageBuffer();

    int32_t nRequestNum = OTAPI_Wrap::getAccountData(
        notaryID, nymID, accountID); // <===== ATTEMPT TO SEND MESSAGE;

    if (OTAPI_Wrap::networkFailure()) {
        otOut << strLocation
        << ": getAccountData message failed due to network error.\n";
        return -1;
    }
    if (-1 == nRequestNum) {
        otOut << strLocation
        << ": Failed to send getAccountData message due to error.\n";
        return -1;
    }
    if (0 == nRequestNum) {
        otOut << strLocation << ": Didn't send getAccountData message, but NO "
                                "error occurred, either. (In this case, SHOULD "
                                "NEVER HAPPEN. Treating as Error.)\n";
        return -1;
    }
    if (nRequestNum < 0) {
        otOut << strLocation
              << ": Unexpected failure sending getAccountData. Request number: "
              << nRequestNum << "\n";
        return -1;
    }

    bWasSentAccount = true;
    bWasSentInbox = true;

    // -1 for error
    //  0 for reply: failure
    //  1 for reply: success
    //
    int32_t nReturn = receiveReplySuccessLowLevel(
        notaryID, nymID, nRequestNum,
        "getInboxAccount"); // <============ RETURN VALUE;

    if (OTAPI_Wrap::networkFailure())
    {
        otOut << strLocation
        << ": Failed to receiveReplySuccessLowLevel due to network error.\n";
        return -1;
    }

    if (nReturn < 0)        // error
    {
        otOut << strLocation << ": Error in getAccountData: " << nReturn
              << ".  (I give up.)\n";
        return -1;
    }

    //      otOut << "IN getInboxAccount " <<
    // getLastReplyReceived());

    // BY this point, we definitely have the request number, which means the
    // message was actually SENT. (At least.) This also means we can use
    // nRequestNum
    // later to query for a copy of that sent message.
    // Let's go ahead, in this case, and remove that now:
    //

    //     int32_t nRemovedSentMsg =
    // OTAPI_Wrap::RemoveSentMessage(Integer.toString(nRequestNum), notaryID,
    // nymID);
    //
    //      // NOTE: The above call is unnecessary, since a successful process
    // means
    //      // we already received the successful server reply, and OT's
    // "ProcessServerReply"
    //      // already removed the sent message from the sent buffer (so no need
    // to do that here.)
    //
    //      if (nRemovedSentMsg < 1) // (not success.)
    //      {
    //          otOut << "getInboxAccount: ERROR:
    // OT_API_RemoveSentMessage returned: " << nRemovedSentMsg);
    //      }

    if (1 != nReturn) {
        otOut << strLocation
              << ": getAccountData failed, returning: " << nReturn << "\n";
        return nReturn;
    }

    // DOWNLOAD THE BOX RECEIPTS.
    if (!insureHaveAllBoxReceipts(notaryID, nymID, accountID,
                                  1)) // <===== nBoxType = 1 aka INBOX;
    {
        otOut << strLocation << ": getAccountData succeeded, but then "
                                "insureHaveAllBoxReceipts failed on the inbox. "
                                "(I give up.)\n";
        return -1;
    }

    if (!insureHaveAllBoxReceipts(notaryID, nymID, accountID,
                                  2)) // <===== nBoxType = 2 aka OUTBOX;
    {
        otOut << strLocation << ": getAccountData succeeded, but then "
                                "insureHaveAllBoxReceipts failed on the "
                                "outbox. (I give up.)\n";
        return -1;
    }

    return 1;
}

// Same as the above function, except you only have to pass the accountID.
// (instead of 3 IDs...)
//
OT_UTILITY_OT bool Utility::getInboxOutboxAccount(const string& accountID)
{
    bool bForceDownload = false;
    return getInboxOutboxAccount(accountID, bForceDownload);
}

OT_UTILITY_OT bool Utility::getInboxOutboxAccount(
    const string& accountID,
    bool bForceDownload) // booleanbForceDownload = false;
{
    string strLocation = "Utility::getInboxOutboxAccount";

    if (!VerifyStringVal(accountID) || accountID.size() < 10) {
        otOut << strLocation << ": invalid accountID: " << accountID << "\n";
        return false;
    }

    string notaryID = OTAPI_Wrap::GetAccountWallet_NotaryID(accountID);
    string nymID = OTAPI_Wrap::GetAccountWallet_NymID(accountID);
    if (!getIntermediaryFiles(notaryID, nymID, accountID, bForceDownload)) {
        otOut << strLocation << ": getIntermediaryFiles failed. (Returning.)\n";
        return false;
    }

    return true;
}

// getInboxAccount()
// Grabs the "Account", which is the intermediary file containing the current
// balance, verified against
// last signed receipt. Server must have your signature on the last balance
// agreement plus, if applicable,
// any inbox receipts (box receipts), also with your signature, in order to
// justify the current balance.
// Any inbox receipts, further, are only valid if they each contain a
// transaction number that was previously
// already signed out to you.
// (As you can see, the "account" is not a list of transactions, as per the
// classical understanding in
// double-entry accounting, but instead it's just a signed balance agreement,
// plus any as-yet-unclosed
// transactions that have cleared since that balance was last signed, and are
// still waiting in the inbox
// for the next balance agreement to be signed when they can be removed.)

// In addition to the "Account" there is also the Inbox itself, as well as all
// of its box receipts.
// The box receipts are stored in abbreviated form in the Inbox itself, with the
// actual full
// versions in separate files. These are retrieved individually from the server
// after the inbox itself
// is, and then each is verified against a hash kept inside its abbreviated
// version.)
// DONE
OT_UTILITY_OT int32_t
    Utility::getInboxAccount(const string& notaryID, const string& nymID,
                             const string& accountID, bool& bWasSentInbox,
                             bool& bWasSentAccount)
{
    bool bForceDownload = false;
    return getInboxAccount(notaryID, nymID, accountID, bWasSentInbox,
                           bWasSentAccount, bForceDownload);
}

} // namespace opentxs
