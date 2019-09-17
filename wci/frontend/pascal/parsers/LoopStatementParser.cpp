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
    AGAIN_SET.insert(PascalTokenType::AGAIN);

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

    // Create LOOP, TEST, and NOT nodes.
    ICodeNode *loop_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_LOOP);
    ICodeNode *test_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_TEST);

    while (token->get_type() != (TokenType) PT_AGAIN){
        //Check for When
    	if(token->get_type() == (TokenType) PT_WHEN){
    	    token = next_token(token);  // consume the WHEN
    	    ExpressionParser expression_parser(this);
    	    test_node->add_child(expression_parser.parse_statement(token));
    	    loop_node->add_child(test_node);

    	    //Check for Break Arrow
    	    if(token->get_type() == (TokenType) PT_BREAK_ARROW){
    	    	token = next_token(token); // Consume Break arrow
    	    }
    	    else{
    	    	error_handler.flag(token, MISSING_BREAK_ARROW, this);
    	    }

    	}
    	else{
    		StatementParser statement_parser(this);
    		loop_node->add_child(statement_parser.parse_statement(token));
    	}

    	//Consume when done
    	token = next_token(token);
    }

    // ensure we have a WHEN
    if (test_node->get_children()->size() == 0) {
        error_handler.flag(token, MISSING_WHEN, this);
    }

    // Synchronize at the AGAIN.
    token = synchronize(AGAIN_SET);
    if (token->get_type() == (TokenType) PT_AGAIN)
    {
        token = next_token(token);  // consume the AGAIN
    }
    else {
        error_handler.flag(token, MISSING_AGAIN, this);
    }

    return loop_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
