#include <stdio.h> 
int main () {
    int ans[] = {
    6         , 8,
    35        , 49,
    204       , 288,
    1189      , 1681,
    6930      , 9800,
    40391     , 57121,
    235416    , 332928,
    1372105   , 1940449,
    7997214   , 11309768,
    46611179  , 65918161};
  int i = 0;
  for (i = 0; i < 10; ++i) {
    printf("%10d%10d\n", ans[i * 2], ans[i * 2 + 1]);
  }
/*  int cnt = 0;
  long long n = 2; 
  while (cnt < 10) {
    // 1. test 
    long long nn = 8 * n * n + 1;
    long long s = sqrt(nn);
    if (s * s == nn) {
      long long t = s - (2 * n + 1);
      if (t % 2 == 0) {
        printf("%10lld%10lld\n", n, n + t / 2); 
        cnt++;
      }
    }
    n++;
  }*/
  return 0;
}
