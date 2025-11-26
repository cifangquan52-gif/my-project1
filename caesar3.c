#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// 清空输入缓冲区
void clear_input_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);//getchar()一次读取一个字符
}

typedef struct node   //链表
{
	struct node*next;
	int number;
} node;

#define word_length 43
typedef struct hash_node   //哈希表
{
	char word [word_length];
	struct hash_node *next;
} hash_node;
const  int N=26;   //26个桶
hash_node *table[N];

void  free_list(node *list);

node* sort_link_list(int number[],int count);
void print_link_list(node* list);
char* jiemi(char *c,int key,char *p);
void change_number(char *p,int result[],int *count);
void sort(int number[],int count);
void print_numbers(int numbers[], int count);
unsigned int hash(const char *word);

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		printf(" only Usage ./caesar3 数字 ");
		return 1;
	}
	char* key = argv[1];
	for(int i =0; i<strlen(key); i++)
	{
		if(!isdigit(key[i]))
		{
			printf("Usage number");
			return 1;
		}
	}
	int k = atoi(key);
	printf("Plaintext:");
	char p[1000];
	fgets(p, sizeof(p), stdin);   //为啥用fgets而不用scanf,因为会造成大量的空格，
	//而且scanf会因为空格停止读取

	char c[1000];
	int length = strlen(p);
	for(int i =0 ; i<length; i++)
	{
		if(islower(p[i]))
			c[i] = (p[i]-'a'+k)%26+'a';
		else if(isupper(p[i]))
		{
			c[i] = (p[i]-'A'+k)%26+'A';
		}
		else
		{
			c[i]=p[i];
		}
	}
	c[length] = '\0';

	printf("Ciphertext:%s\n", c);

	printf("想要解密的话，输入钥匙");
	int new_k=0;
	printf("new_key: ");

	// 在读取密钥前清空缓冲区
	clear_input_buffer();//为啥多次清空，防止程序自动进行

	scanf("%d",&new_k);
	clear_input_buffer();  // 读取后再次清空

	char jiemi_result[1000];
	jiemi(c, new_k, jiemi_result);

	if(strcmp(p, jiemi_result) == 0)
	{
		printf("解密成功: %s\n", jiemi_result);
	}
	else
	{
		printf("key is error\n");
		return 1;
	}

	printf("1.输入1，想要把明文字母变为数组数字并排序\n");
	printf("2.输入2想用哈希表搜索明文单词，输入2\n");
	printf("3.输入3想要把明文字母变为数组数字并<< 链表插入>>排序，输入3\n");
	printf("button= ");

	int button=0;
	scanf("%d",&button);
	clear_input_buffer();

	if(button == 1)
	{
		int result[1000];
		int count = 0;
		change_number(p, result, &count);

		printf("转换后的数字: ");
		print_numbers(result, count);

		sort(result, count);
		printf("排序后的数字: ");
		print_numbers(result, count);
	}
	else if(button==2)
	{
		printf("哈希查找，待完善");

	}
	else if(button==3)
	{
		int result[1000];
		int count = 0;
		change_number(p, result, &count);

		printf("转换后的数字: ");
		print_numbers(result, count);
		node *list=sort_link_list(result,count);
		print_link_list(list);
		free_list(list);



	}

	return 0;
}

char* jiemi(char *c,int key,char *p)   //解密函数
{
	int length=strlen(c);
	for(int i =0 ; i<length; i++)
	{
		if(islower(c[i]))
			p[i] = (c[i]-'a'-key+26)%26+'a';//为啥加26，因为C语言中负数取模的结果是负数，这会导致数组越界和错误
		else if(isupper(c[i]))
		{
			p[i] = (c[i]-'A'-key+26)%26+'A';
		}
		else
		{
			p[i]=c[i];
		}
	}
	p[length] = '\0';  // 添加终止符
	return p;
}

void change_number(char *p,int result[],int *count)
{
	// 用解引用运算符让这个地址存储0
	*count =0;
	for(int i =0; i<strlen(p); i++)
	{
		if(islower(p[i]))
		{
			result[*count]=(p[i]);
			(*count)++;//为啥是(*count)++而不是count++，因为后者会改变地址，指向垃圾值，前者不会改变地址，会增加数字
		}
		else if( isupper(p[i]))
		{
			result[*count]=(p[i]);
			(*count)++;
		}
	}
}

void sort(int number[],int count)   //select排序函数
{
	for(int i =0; i<count; i++)
	{
		int min=i;
		for(int j = i+1; j<count; j++)
		{
			if(number[min]>number[j])
			{
				min=j;
			}
		}
		if(min!=i)
		{
			int temp=number[i];
			number[i]=number[min];
			number[min]=temp;
		}
	}
}
//打印函数
void print_numbers(int numbers[], int count)
{
	for(int i= 0; i<count; i++)
	{
		printf("%d ", numbers[i]);
	}
	printf("\n");
}

unsigned int hash(const char *word)   //djb2算法算哈希值
{
	unsigned long hash = 5381;
	int c;
	while ((c = toupper(*word++)))
	{
		hash = ((hash << 5) + hash) + c;
	}
	return hash%26;
}
node* sort_link_list(int num[],int count)   //插入排序链表函数
{
	node* list=NULL;
	for(int i = 0; i<count; i++)
	{
		node* n=malloc(sizeof(node));
		if(n==NULL)
		{
			printf("error");
			return NULL;
		}
		n->number=num[i];
		n->next=NULL;
		if(list==NULL)
		{
			list=n;
		}
		else if(n->number < list->number)
		{
			n->next=list;
			list=n;


		}
		else
		{
			for(node *ptr=list; ptr!=NULL; ptr=ptr->next)
			{
				if(ptr->next==NULL)
				{
					ptr->next=n;
					break;
				}
				else if(n->number<ptr->next->number)
				{
					n->next=ptr->next;
					ptr->next=n;
					break;
				}
			}
		}

	}
	return list;
}

void print_link_list(node* list)   //打印链表函数
{
	for (node *ptr = list; ptr != NULL; ptr = ptr->next)
	{
		printf("%i\n", ptr->number);
	}
	printf("\n");
}
void free_list(node *list)   //释放链表函数
{
	node *current=list;
	while(current!=NULL)
	{
		node*new_next =current->next;
		free(current);
		current=new_next;
	}
}
