extern long
fib0 (long n)
{
body:
  goto if_cond;

if_cond:
  if (n < (long)2) goto if_body; else goto next;

if_body:
  return n;

next:
  return fib0 ((n - (long)1)) + fib0 ((n - (long)2));
}

