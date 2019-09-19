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
#include "WhenStatementParser.h"
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

bool WhenStatementParser::INITIALIZED = false;

EnumSet<PascalTokenType> WhenStatementParser::BREAK_ARROW_SET;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    BREAK_ARROW_SET = StatementParser::STMT_START_SET;
    BREAK_ARROW_SET.insert(PascalTokenType::BREAK_ARROW);

    EnumSet<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
    	BREAK_ARROW_SET.insert(*it);
    }

    INITIALIZED = true;
}

WhenStatementParser::WhenStatementParser(PascalParserTD *parent)
    : StatementParser(parent)
{
    initialize();
}

ICodeNode *WhenStatementParser::parse_statement(Token *token) throw (string)
{
    token = next_token(token);  // consume the When
    // Create TEST node for attaching to parent
    ICodeNode *test_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_TEST);




        ExpressionParser expression_parser(this);
        test_node->add_child(expression_parser.parse_statement(token));

        token = synchronize(BREAK_ARROW_SET);
        if (token->get_type() == (TokenType) PT_BREAK_ARROW)
        {
            token = next_token(token);  // consume the BREAK_ARROW
        }
        else {
            error_handler.flag(token, MISSING_BREAK_ARROW, this);
        }






    return test_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
