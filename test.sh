#!/bin/sh

PROG="./codeRunner"

##########################
##     test case 1
##########################
$PROG  --no-prompt > output/case1.out <<EOF
/* 9x9 multiplication tables */
int i,j;
for (i=1;i<=9;i++)
for (j=1;j<=9;j++)
{
    printf("%d x %d = %d\t",j,i,i*j);

    /* when equal, start a new line */
    if (i==j)
    {
                    printf("\n");
                    break;
    }
}
quit
EOF

diff ./output/case1.out ./expected/case1.out

if [ $? -eq 0 ] ; then
		echo "test case 1 ok !"
else
		echo "test case 1 failed!"
fi

##########################
##     test case 1
##########################
$PROG  --no-prompt > output/case2.out <<EOF
int i,j,k;
printf("\n");
for(i=1;i<5;i++)
for(j=1;j<5;j++)
for (k=1;k<5;k++)
{
	if (i!=k&&i!=j&&j!=k)
	printf("%d,%d,%d\n",i,j,k);
}
quit
EOF


diff ./output/case2.out ./expected/case2.out

if [ $? -eq 0 ] ; then
		echo "test case 2 ok !"
else
		echo "test case 2 failed!"
fi
