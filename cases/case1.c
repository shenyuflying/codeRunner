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
