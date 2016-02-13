int var;
int new_var = 10;

void my_func(int *i){
	*i = 5;
}

int main(int argc, char *argv[]){
	int stack_var = 0;
	my_func(&stack_var);
}
