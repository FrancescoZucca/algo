#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char* cercaRegexp(char*, char*);
bool matchParentheses(char, char*);

int main(){
	char buf[200], regbuf[200];
	fgets(buf, 200, stdin);
	fgets(regbuf, 200, stdin);
	printf("%s", cercaRegexp(buf, regbuf));
	return 0;
}

char* cercaRegexp(char* src, char* regexp) {
	char* ret = NULL, *s = src, *r = regexp, c;
	bool match = false;
	int matches = 0;
		
	while ( (c = *(s++))!=0 && *r!=0){
			switch (*r) {
				case '.':
					match = true;
					r++;
					matches++;
					break;
				case '\n':
					r++;
					continue;
				case '[':
					match = matchParentheses(c, r);
					while (*r++ != ']') if (*r==0) exit(-1);
					break;
				case '\\':
					char b = *(++r);
					if (b=='A') match = c>='A'&&c<='Z';
					else if (b=='a') match = c>='a'&&c<='z';
					else match = c == b;
					r++;
					break;
				default:
					match = c == *r;
					r++;
					break;
		}

		if (match && ret == NULL)
		       ret = s - 1;
		if (!match) {
			r = regexp;
			matches = 0;
			if (ret != NULL) {
				s = ret + 1;
				ret = NULL;
			}
		} else {
			matches++;
		}
	}

	if (match && *r == '\0') {
		if (ret != NULL) ret[matches] = 0;
		return ret;
	}

	return NULL;
}

bool matchParentheses(char c, char* regexp){
	bool reversed = false;
	char r;
	if (*++regexp == '^'){
		reversed = true;
		regexp++;
	}
	while ((r = *(regexp++)) != ']'){
		if (r == c) reversed = !reversed;
	}

	return reversed;
}
