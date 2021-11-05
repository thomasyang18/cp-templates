import java.util.*;
public class digit_dp {
	long[][] dp = new long[20][2];
	int[] r = new int[20];
	int n;
	public digit_dp(long a) {
		String s = ""+a; 
		n = s.length();
		for (int i = 0; i < n; i++) r[i] = s.charAt(i)-'0';
		for (long[] b: dp) Arrays.fill(b, -1);
	}
	
	public long solve(int pos, int is_eq) {
		if (pos == n) return 1;
		if (dp[pos][is_eq] == -1) {
			dp[pos][is_eq] = 0;
			for (int i = 0; i <= (is_eq==1?r[pos]:9); i++) {
				dp[pos][is_eq] += solve(pos+1, (is_eq == 1 && i == r[pos]) ? 1:0);
			}
		}
		return dp[pos][is_eq];
	}
}