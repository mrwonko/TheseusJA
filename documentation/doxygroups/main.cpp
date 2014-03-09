/** @defgroup GroupA First group */
/** @defgroup GroupB Second group */
/**	@brief Function A
	
	A function.
	@ingroup GroupA */
void FuncA()
{
}

/**	@brief Function B
	
	Not A function.
	@ingroup GroupB */
void FuncB()
{
	FuncA();
}