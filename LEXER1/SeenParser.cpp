#include "SeenParser.h"
Expression* SeenParser::ArithmeticExpr() {
	Expression* t = Term();
	while (Lookahead->IS("+") || Lookahead->IS("-")) {
		string theOperator = Lookahead->Lexeme;
		Match(Lookahead->Lexeme);
		Expression* t2 = Term();
		// t = new BinaryPrimitive(theOperator, t, t2);
		t = BinaryPrimitive(theOperator, t, t2);
	}
	return t;
 }
Expression* SeenParser::Term() {
	Expression* t = Primary();
	while (Lookahead->IS("*") || Lookahead->IS("/")) {
		string theOperator = Lookahead->Lexeme;
		Match(Lookahead->Lexeme);
		Expression* t2 = Primary();
		// t = new BinaryPrimitive(theOperator, t, t2);
		t = BinaryPrimitive(theOperator, t, t2);
	}
	return t;
}
Expression* SeenParser::Primary() {
	Expression* t = PrimaryNonApplication();
	while (Lookahead->IS(TOKEN_LPAREN)) {
		ArgList* args = Args();
		//t = new FuncApplication(t, args);
		t = FuncApplication(t, args);
	}
	return t;
}
Expression* SeenParser::PrimaryNonApplication() {
	Expression* t = NULL;
	if (Lookahead->IS(TOKEN_LPAREN)) {
		Match(TOKEN_LPAREN);
		t = Expr();
		Match(TOKEN_RPAREN);
		return t;
	}
	if (Lookahead->IS(TOKEN_INT_LITERAL)) {
		int value = atoi(Lookahead->Lexeme.c_str());
		Match(TOKEN_INT_LITERAL);
		//t = new IntLiteral(value);
		t = IntLiteral(value);
		return t;
	}
	if (Lookahead->IS(TOKEN_STRING_LITERAL)) {
		string value = Lookahead->Lexeme;
		Match(TOKEN_STRING_LITERAL);
		//t = new StrLiteral(value);
		t = StrLiteral(value);
		return t;
	}
	if (Lookahead->IS(TOKEN_LET)) {
		Match(TOKEN_LET);
		BindList* lst = Bindings();
		Match(TOKEN_LET);
		//t = new LetExpression(t, lst);
		t = LetExpression(t, lst);
		return t;
	}
	if (Lookahead->IS(TOKEN_FUNC)) {
		Match(TOKEN_FUNC);
		ParamList* lst = Params();
		Match(TOKEN_LPAREN);
		Expression* exp = Expr();
		Match(TOKEN_RPAREN);
		//t = new FunctionExpression(t, lst);
		t = FunctionExpression(t, lst);
		return t;
	}
	//---------EXPR()--------
	//|       (a,b,c,d)     |
	//|           ,         |
	//|       ,      d      |
	//|    ,    c           |
	//|  a   b              |
	//------------------------
	if (Lookahead->IS(TOKEN_IF)) {
		Match(TOKEN_IF);
		Match(TOKEN_LPAREN);
		Expression* condition = Expr();
		Match(TOKEN_RPAREN);
		Match(TOKEN_LBRACE);
		Expression* thenExp = Expr();
		Match(TOKEN_RBRACE);
		Match(TOKEN_ELSE);
		Match(TOKEN_LBRACE);
		Expression* elseExp = Expr();
		Match(TOKEN_RBRACE);
		//t = new ifExpression(condition, thenExp, elseExp);
		t = IfExpression(condition, thenExp, elseExp);
		return t;
	}
}