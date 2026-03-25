# 1 "<built-in>"
# 1 "crash.c"
typedef struct X X;
typedef struct Y Y;

Y* X_to_Y(X* x) __attribute__((ownership_returns(Y))) __attribute__((ownership_takes(X, 1)));

int main() {
	return X_to_Y(0) != 0;
}
