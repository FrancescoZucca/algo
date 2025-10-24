#include <stdio.h>

int gcd(int a, int b) {
	if (a < b) return gcd(b, a);
	if (a == b) return a;

	if (a == 1 || b == 1) return 1;

	if (a % 2) {
		if (b % 2)
			return gcd((a - b) / 2, b);
		return gcd(a, b / 2);
	}
	if (b % 2)
		return gcd(a / 2, b);
	return 2 * gcd(a / 2, b / 2);
}	

int main() {
	printf("%d", gcd(351, 123));
}
