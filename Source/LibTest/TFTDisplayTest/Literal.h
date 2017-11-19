#ifndef LITERAL_H
#define LITERAL_H

// 
// NOTE:
//  LITERAL_0 ~ LITERAL_9 ÇÕòAî‘Ç…Ç∑ÇÈÇ±Ç∆.
//  LITERAL_0Ç™ç≈èâÇ…óàÇÈÇ±Ç∆Ç™ñ]Ç‹ÇµÇ¢.
//

enum LITERAL {
	LITERAL_0,
	LITERAL_1,
	LITERAL_2,
	LITERAL_3,
	LITERAL_4,
	LITERAL_5,
	LITERAL_6,
	LITERAL_7,
	LITERAL_8,
	LITERAL_9,
	LITERAL_DOT,
	LITERAL_PLUS,
	LITERAL_MINUS,
	LITERAL_MULTIPLY,
	LITERAL_DIVIDE,
	LITERAL_EQUAL,
	LITERAL_LEFT_BRACKET,
	LITERAL_RIGHT_BRACKET,
	LITERAL_UNKNOWN
};


String LiteralToString(LITERAL literalToStr) {
	String str;

	switch (literalToStr) {
	case LITERAL::LITERAL_0:
		str = F("0");
		break;
	case LITERAL::LITERAL_1:
		str = F("1");
		break;
	case LITERAL::LITERAL_2:
		str = F("2");
		break;
	case LITERAL::LITERAL_3:
		str = F("3");
		break;
	case LITERAL::LITERAL_4:
		str = F("4");
		break;
	case LITERAL::LITERAL_5:
		str = F("5");
		break;
	case LITERAL::LITERAL_6:
		str = F("6");
		break;
	case LITERAL::LITERAL_7:
		str = F("7");
		break;
	case LITERAL::LITERAL_8:
		str = F("8");
		break;
	case LITERAL::LITERAL_9:
		str = F("9");
		break;
	case LITERAL::LITERAL_PLUS:
		str = F("+");
		break;
	case LITERAL::LITERAL_MINUS:
		str = F("-");
		break;
	case LITERAL::LITERAL_MULTIPLY:
		str = F("*");
		break;
	case LITERAL::LITERAL_DIVIDE:
		str = F("/");
		break;
	case LITERAL::LITERAL_EQUAL:
		str = F("=");
		break;
	case LITERAL::LITERAL_LEFT_BRACKET:
		str = F("(");
		break;
	case LITERAL::LITERAL_RIGHT_BRACKET:
		str = F(")");
		break;
	case LITERAL::LITERAL_DOT:
		str = F(".");
		break;
	case LITERAL::LITERAL_UNKNOWN:
		str = F("?");
		break;
	}

	return str;
}



#endif