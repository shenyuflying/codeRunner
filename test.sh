#!/bin/sh
########################################
#####   regression test for codeRunner
########################################

PROG="./bin/codeRunner"
ARGS="--no-prompt --no-readline"
CASES_DIR="./test/cases"
EXPECTED_DIR="./test/expected"
OUTPUT_DIR="./test/output"

numCases=0
numCasesOk=0
numCasesFailed=0

for case in `ls ./test/cases`
do
	echo -n "test $case ... "
	numCases=`expr $numCases + 1`
	name=`basename $case .c`
	cat $CASES_DIR/$case | $PROG $ARGS > $OUTPUT_DIR/$name.out
	diff $OUTPUT_DIR/$name.out $EXPECTED_DIR/$name.out
	if [ $? -eq 0 ] ; then
		echo "ok !"
		numCasesOk=`expr $numCasesOk + 1`
	else
		echo "failed!"
		numCasesFailed=`expr $numCasesFailed + 1`
	fi
done

echo "total: $numCases"
echo "passed: $numCasesOk"
echo "failed: $numCasesFailed"

