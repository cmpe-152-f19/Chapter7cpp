/**
 * <h1>LoopStatementParser</h1>
 *
 * <p>Parse a Pascal Loop statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <set>
#include "LoopStatementParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "../PascalParserTD.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../Token.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

bool LoopStatementParser::INITIALIZED = false;

EnumSet<PascalTokenType> LoopStatementParser::AGAIN_SET;

void LoopStatementParser::initialize()
{
    if (INITIALIZED) return;

    AGAIN_SET = StatementParser::STMT_START_SET;
    AGAIN_SET.insert(PT_AGAIN);

    EnumSet<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        AGAIN_SET.insert(*it);
    }

    INITIALIZED = true;
}

LoopStatementParser::LoopStatementParser(PascalParserTD *parent)
    : StatementParser(parent)
{
    initialize();
}

ICodeNode *LoopStatementParser::parse_statement(Token *token) throw (string)
{
    token = next_token(token);  // consume the Loop

    // Create LOOP, TEST
    ICodeNode *loop_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_LOOP);
    StatementParser statement_parser(this);

    statement_parser.parse_list(token, loop_node, PT_AGAIN, MISSING_AGAIN);

    return loop_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
