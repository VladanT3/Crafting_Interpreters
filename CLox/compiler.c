#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
	Token current;
	Token previous;
	bool had_error;
	bool panic_mode;
} Parser;

Parser parser;

Chunk* compilingChunk;

static Chunk* currentChunk() {
	return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
	if (parser.panic_mode) {
		return;
	}
	parser.panic_mode = true;
	fprintf(stderr, "[line %d] Error", token->line);

	if (token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	} else if (token->type == TOKEN_ERROR) {
	} else {
		fprintf(stderr, " at '%*.s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser.had_error = true;
}

static void errorAtCurrent(const char* message) {
	errorAt(&parser.current, message);
}

static void error(const char* message) {
	errorAt(&parser.previous, message);
}

static void advance() {
	parser.previous = parser.current;

	for (;;) {
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) {
			break;
		}
		errorAtCurrent(parser.current.start);
	}
}

static void consume(TokenType type, const char* message) {
	if (parser.current.type == type) {
		advance();
		return;
	}

	errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
	writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitReturn() {
	emitByte(OP_RETURN);
}

static void endCompiler() {
	emitReturn();
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
	emitByte(byte1);
	emitByte(byte2);
}

bool compile(const char* source, Chunk* chunk) {
	initScanner(source);
	compilingChunk = chunk;

	parser.had_error = false;
	parser.panic_mode = false;

	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");

	endCompiler();
	return !parser.had_error;
}
