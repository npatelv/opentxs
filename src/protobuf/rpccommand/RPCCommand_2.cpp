// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "opentxs/Proto.hpp"  // IWYU pragma: associated

#include <string>

#include "opentxs/protobuf/Basic.hpp"
#include "opentxs/protobuf/RPCEnums.pb.h"
#include "opentxs/protobuf/verify/RPCCommand.hpp"
#include "opentxs/protobuf/verify/VerifyContacts.hpp"
#include "opentxs/protobuf/verify/VerifyRPC.hpp"
#include "protobuf/Check.hpp"

#define PROTO_NAME "RPC command"

namespace opentxs::proto
{
bool CheckProto_2(const RPCCommand& input, const bool silent)
{
    CHECK_IDENTIFIER(cookie)
    CHECK_EXISTS(type)

    switch (input.type()) {
        case RPCCOMMAND_ADDCLIENTSESSION: {
            if (-1 != input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_SUBOBJECTS(arg, RPCCommandAllowedAPIArgument());
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ADDSERVERSESSION: {
            if (-1 != input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_SUBOBJECTS(arg, RPCCommandAllowedAPIArgument());
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTCLIENTSESSIONS: {
            if (-1 != input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTSERVERSESSIONS: {
            if (-1 != input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_IMPORTHDSEED: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_SUBOBJECT(hdseed, RPCCommandAllowedHDSeed());
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTHDSEEDS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETHDSEED: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_CREATENYM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_SUBOBJECT(createnym, RPCCommandAllowedCreateNym());
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTNYMS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETNYM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ADDCLAIM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_SUBOBJECTS(claim, RPCCommandAllowedAddClaim());
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_DELETECLAIM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_IMPORTSERVERCONTRACT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_HAVE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_SUBOBJECTS(server, RPCCommandAllowedServerContract());
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTSERVERCONTRACTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_REGISTERNYM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_IDENTIFIER(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_CREATEUNITDEFINITION: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_SUBOBJECT(
                createunit, RPCCommandAllowedCreateInstrumentDefinition());
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTUNITDEFINITIONS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ISSUEUNITDEFINITION: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_IDENTIFIER(notary);
            CHECK_IDENTIFIER(unit);

            if (0 != input.identifier().size()) {
                CHECK_SIZE(identifier, 1);
                const auto size = input.identifier(0).size();

                if (MAX_VALID_CONTACT_VALUE < size) {
                    FAIL_2("invalid alias size", size);
                }
            }

            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_CREATEACCOUNT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_IDENTIFIER(notary);
            CHECK_IDENTIFIER(unit);

            if (0 != input.identifier().size()) {
                CHECK_SIZE(identifier, 1);
                const auto size = input.identifier(0).size();

                if (MAX_VALID_CONTACT_VALUE < size) {
                    FAIL_2("invalid alias size", size);
                }
            }

            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTACCOUNTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            OPTIONAL_IDENTIFIER(notary);
            OPTIONAL_IDENTIFIER(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETACCOUNTBALANCE: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETACCOUNTACTIVITY: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_SENDPAYMENT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_SUBOBJECT(sendpayment, RPCCommandAllowedSendPayment());
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_MOVEFUNDS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_SUBOBJECT(movefunds, RPCCommandAllowedSendPayment());
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ADDCONTACT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_SUBOBJECTS(addcontact, RPCCommandAllowedAddContact());
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LISTCONTACTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETCONTACT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ADDCONTACTCLAIM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_SUBOBJECTS(claim, RPCCommandAllowedAddClaim());
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_DELETECONTACTCLAIM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_VERIFYCLAIM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_SUBOBJECTS(verifyclaim, RPCCommandAllowedVerifyClaim());
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ACCEPTVERIFICATION: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_IDENTIFIER(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_SUBOBJECTS_VA(
                acceptverification,
                RPCCommandAllowedVerification(),
                VerificationType::Indexed);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_SENDCONTACTMESSAGE: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_SUBOBJECTS(sendmessage, RPCCommandAllowedSendMessage());
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETCONTACTACTIVITY: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETSERVERCONTRACT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETPENDINGPAYMENTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_HAVE(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_ACCEPTPENDINGPAYMENTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_SUBOBJECTS(
                acceptpendingpayment, RPCCommandAllowedAcceptPendingPayment());
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_CREATECOMPATIBLEACCOUNT:
        case RPCCOMMAND_GETCOMPATIBLEACCOUNTS: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_HAVE(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETWORKFLOW: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_SUBOBJECTS(arg, RPCCommandAllowedAPIArgument());
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_SUBOBJECTS(getworkflow, RPCCommandAllowedGetWorkflow());
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETSERVERPASSWORD: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETADMINNYM: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETUNITDEFINITION: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_GETTRANSACTIONDATA: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_HAVE(identifier);
            CHECK_IDENTIFIERS(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_LOOKUPACCOUNTID: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_NAME(param);
            CHECK_NONE(modifyaccount);
        } break;
        case RPCCOMMAND_RENAMEACCOUNT: {
            if (0 > input.session()) { FAIL_1("invalid session"); }

            OPTIONAL_IDENTIFIERS(associatenym);
            CHECK_EXCLUDED(owner);
            CHECK_EXCLUDED(notary);
            CHECK_EXCLUDED(unit);
            CHECK_NONE(identifier);
            CHECK_NONE(arg);
            CHECK_EXCLUDED(hdseed);
            CHECK_EXCLUDED(createnym);
            CHECK_NONE(claim);
            CHECK_NONE(server);
            CHECK_EXCLUDED(createunit);
            CHECK_EXCLUDED(sendpayment);
            CHECK_EXCLUDED(movefunds);
            CHECK_NONE(addcontact);
            CHECK_NONE(verifyclaim);
            CHECK_NONE(sendmessage);
            CHECK_NONE(acceptverification);
            CHECK_NONE(acceptpendingpayment);
            CHECK_NONE(getworkflow);
            CHECK_EXCLUDED(param);
            CHECK_SUBOBJECTS(modifyaccount, RPCCommandAllowedModifyAccount());
        } break;
        case RPCCOMMAND_ERROR:
        default: {
            FAIL_1("invalid type")
        }
    }

    return true;
}

bool CheckProto_3(const RPCCommand& input, const bool silent)
{
    return CheckProto_2(input, silent);
}

bool CheckProto_4(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(4)
}

bool CheckProto_5(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(5)
}

bool CheckProto_6(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(6)
}

bool CheckProto_7(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(7)
}

bool CheckProto_8(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(8)
}

bool CheckProto_9(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(9)
}

bool CheckProto_10(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(10)
}

bool CheckProto_11(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(11)
}

bool CheckProto_12(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(12)
}

bool CheckProto_13(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(13)
}

bool CheckProto_14(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(14)
}

bool CheckProto_15(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(15)
}

bool CheckProto_16(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(16)
}

bool CheckProto_17(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(17)
}

bool CheckProto_18(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(18)
}

bool CheckProto_19(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(19)
}

bool CheckProto_20(const RPCCommand& input, const bool silent)
{
    UNDEFINED_VERSION(20)
}
}  // namespace opentxs::proto
