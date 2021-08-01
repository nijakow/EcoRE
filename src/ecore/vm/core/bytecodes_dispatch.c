
#define DEFDISPATCH(N)  [Eco_Bytecode_ ## N] = &&dispatch_label_ ## N

static void* DISPATCH_TABLE[] = {
    DEFDISPATCH(NOOP),
    DEFDISPATCH(CONST),
    DEFDISPATCH(PUSHC),
    DEFDISPATCH(PUSH),
    DEFDISPATCH(POP),
    DEFDISPATCH(DROP),
    DEFDISPATCH(DUP),
    DEFDISPATCH(R2R),
    DEFDISPATCH(R2L),
    DEFDISPATCH(L2R),
    DEFDISPATCH(BUILTIN),
    DEFDISPATCH(SEND),
    DEFDISPATCH(ASSIGN),
    DEFDISPATCH(RETURN),
    DEFDISPATCH(MAKE_CLOSURE),
};
