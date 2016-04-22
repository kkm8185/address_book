//////////////////////////////////////////////////////////////////////////
// File Name	: func.c												//
// Date	 		: 2014.06.04											//
// Compiler 	: gcc 4.8.4												//
// Os	 		: LinuxMint												//
// Author		: Kim kyung min											//
//----------------------------------------------------------------------//
// ver			: 1.0.1													//
// Description	: Address Book											//
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#define LAST_NODE ((Person*)-1)							// ��ũ������
extern Person *person_head;								// �ٸ������� Person struct�� ���� ����.
void argc_check(int,char* argv[]);						// �Է°� Ȯ��
void small_letter_change(char *p);						// �빮�ڸ� �ҹ��ڷ� ��ȯ
char small_letter_change2(char);						// �빮�ڸ� �ҹ��ڷ� ��ȯ
char big_letter_change2(char);							// �ҹ��ڸ� �빮�ڷ� ��ȯ
char bar_change2(char);
char blank_change2(char);
int name_compare(char *p,char *pp);						// �̸� �� ( �ҹ��ڷ� �ٲ㼭 ���� )
void write_file_add(char*,char*);						// input ���Ͽ� ������ �߰�
void write_file(int);									// �ش� ������ ����, write ( ������ ����, �����ϸ� �߰��ۼ� )
void write_person(Person *temp);						// �ش� ������ ����, write ( �̸� �� �޴�����ȣ )
void struct_sort();										// �����ϱ�
void struct_data_save(Person *a,int flag,char* temp);	// ����ü�� ������ �����ϱ�
void blank_clear(char* temp);							// ������ ���ڰ� �����ϰ��, ���� ����
void struct_output();									// ����ü ȭ�鿡 ���
void struct_clear();									// ����ü ����
Person* struct_reset(Person *a);						// �ش� ����ü �� �Ҵ�
void struct_input();									// �ش� ������ �о�, struct �Ҵ�
void struct_del(Person *a);								// �ش� ����ü ����
void struct_find(int flag,char*,int);					// �ش� ����ü ã��
void struct_add(char *name, char* phone);				// �ش� ����ü ����
void argc0_system(char* argv[]);						// �Է��� �߸��ϰų� �Ѱ��� ������� �ش� ���� ���
int name_check(char* argv[]);							// argv �ι�° ���� name or phone Ȯ��
char* make_temp(int argc,char* argv[]);					// argv�� �Էµ� �����͸� temp�� ����
void argc0_help(char* argv[]);							// argv �߸� �Է����� ��, ���� ���
char* make_add(int argc, char* argv[], char* temp);		// -add�� ���� ����

int name_check(char* argv[]){
	if(strcmp(argv[2],"name")==0)		return 0;		// argv[2] ���� name�� ���, 0
	else if(strcmp(argv[2],"phone")==0)	return 1;		// argv[2] ���� phone�� ���, 1
	else					argc0_system(argv);			// �׷��� ������쿡�� argc0_system ȣ���Ͽ� �߸��� �Է¿� ���� ���� ȣ��
}
// name or phone word check.
char* make_add(int argc,char* argv[],char* temp){		// argv���� �̸����� ����
	char name[20];
	int i;
	memset(name,0,20);
	// name reset
	strcpy(name,argv[2]);								// �̸��κ��� [2]�� �ϴ� ����.
	for(i=3;;i++){
		if(argv[i][0]>='0'&&argv[i][0]<='9')	break;	// [i]���� ���ڰ� �߰ߵȴٸ� �ݺ��� �׸�.
		if(argv[i]==NULL){								// ���ڴ� �ƴѵ�, [i]�� null�ϰ��,
			struct_clear();								// �� ����ü ����
			argc0_system(argv);							// �߸��� �ɼǿ� ���� ����
		}
		strcat(name," ");								// ���ڵ� �ƴϰ�, null�� �ƴ϶��, " "�� ���� ��������ش�.
		strcat(name,argv[i]);							// temp�� ����.

	}
	// not number ? save word. but number ? stop save word.
	if(argv[i]==NULL){									// �̸� ����Ϸ�, �׷���, �������� �������� �ʴٸ�?
		struct_clear();									// �� ����ü ����
		argc0_system(argv);								// �߸��� �ɼǿ� ���� ����
	}
	strcpy(temp,argv[i]);								// temp��� ������ �޴��� ��ȣ ù�κ�(Ȥ�� ���κ�) ����.
	i++;
	for(;i<argc;i++){									// �ݺ��� ���� �޴��� ��ȣ ��� ����
		strcat(temp," ");
		strcat(temp,argv[i]);
	}
	// number ? save word.
	return name;										// name�� return
	// return name value.(temp is Reference)
}
char* make_temp(int argc,char* argv[]){
	char temp[20];
	int i;
	memset(temp,0,20);
	// temp reset
	if(argc<=3){										// argc ������ 3������ ������ �߸��� �ɼ�
		struct_clear();
		argc0_system(argv);
	}
	if(argc>=4){										// argc ������ 4������ ���ų� ������
		strcpy(temp,argv[3]);
		for(i=4;i<argc;i++){
			strcat(temp," ");							// �ɼ� ���� �̸� Ȥ�� �޴��� ��ȣ�� ���� ������ ����.
			strcat(temp,argv[i]);
		}
	}
	return temp;
	// save word and return temp value
}
void struct_find(int flag,char *temp,int flag2){
//	flag == 0 -> name value find.
//	flag == 1 -> phone value find.
//	flag2 == 0 -> find option
//	flag2 == 1 -> del option
	char tmp[20],tmp2[20];
	Person *p=person_head;					// head�� p�� �ӽ�����
	int i=0,connection_flag=0,j=0;
	while(1){
		if(p==NULL){						// p�� NULL�̸�
			if(flag2==1)		printf("not delete data\n");	// del �ɼ��̶�� �ش� ���� ���
			break;
		}
		strcpy(tmp,temp);					// temp���� tmp�� ����
		// change tmp value reset
		if(flag==0){						// name�� ���õ� ���̶�� 
			if(p==LAST_NODE)						break;		// p�� ������ ����� ����
			for(i=0,j=0,connection_flag=0;;i++,j++){
				if(i==0)	strcpy(tmp2,p->name);				// tmp2�� �ش� ����� �̸��� ����.
				if(tmp[i]=='*'){								// ���ϵ�ī�� ����(*)�� �����Ѵٸ�,
					if(flag2!=1){								// delete�ɼ��� �ƴ϶��
						printf("name : %s, phone : %s\n",p->name,p->phone);	// ȭ�� ���
						write_person(p);						// ���Ͽ� �ۼ�
					}
					if(flag2==1){								// delete �ɼ��̶��
						printf("name : %s, phone : %s\n",p->name,p->phone);	// ȭ�����
						struct_del(p);							// �ش� ����ü ����.
					}
					break;
				}
				// * -> all output
				else if(tmp[i]==tmp2[j])				connection_flag=1;
				// ã�� ������� p�� �̸��� ���Ͽ� ������ connection_flag 1������ ���ٶ�� ���� ����
				else if(tmp[i]=='?'){
					connection_flag=1;
				}
				// ���ϵ�ī�� (?)���ڸ� connection_flag 1������ ���ٶ�� ���� ����
				else{
					if(tmp[i]!=tmp2[j]){	// �����ʴٸ�
						if(tmp[i]==' ')				i++;	// tmp�� ������ �ִٸ� �������ڷ� �̵�
						if(tmp2[j]==' ')			j++;	// tmp2�� ������ �ִٸ� �������ڷ� �̵�
						if(tmp[i]!=tmp2[j]){	// �ٲ�µ��� ���� �ʴٸ�
							if(tmp[i]>='A'&&tmp[i]<='Z')		tmp[i]=small_letter_change2(tmp[i]);// �빮�ڸ� �ҹ��ڷ� ����
							else if(tmp[i]>='a'&&tmp[i]<='z')	tmp[i]=big_letter_change2(tmp[i]);	// �ҹ��ڸ� �빮�ڷ� ����
						}
						if(tmp[i]==tmp2[j])			connection_flag=1;	// ������ connection_flag 1�� ���� ���ٶ�� ���� ����
						else						connection_flag=0;	// �ٸ��� connection_flag 0�� ���� �ٸ��ٶ�� ���� ����
					}
				}
				if(tmp[i]==0&&connection_flag==1){	// ���� ���ڰ� �� ����, connection_flag�� 1�� ���ٸ� �������̴�.
					if(flag2!=1){					// delete �ɼ��� �ƴ϶��
						printf("name : %s, phone : %s\n",p->name,p->phone);
						write_person(p);			// �ش� person ���Ͽ� �ۼ�
					}
					if(flag2==1){					// delete �ɼ��̶��
						printf("name : %s, phone : %s\n",p->name,p->phone);
						struct_del(p);				// �ش� ����ü���� ����
					}
					break;
				}
				else if(connection_flag==0)				break;
			}
			p=p->next;								// ���� ����ü�� �̵�
		}
		else if(flag==1){							// �޴����� ���õ� ���̶��
			if(p==LAST_NODE)						break;
			for(i=0,j=0,connection_flag=0;;i++,j++){
				if(i==0)						strcpy(tmp2,p->phone);
				if(tmp[i]=='*'){					// ���ϵ� ī���ϰ�쿡��
					if(flag2!=1){					// delete�� �ƴϸ�
						printf("name : %s, phone : %s\n",p->name,p->phone);
						write_person(p);			// �ش� ����ü ���Ͽ� �ۼ�
					}
					if(flag2==1){					// delete���
						printf("name : %s, phone : %s\n",p->name,p->phone);
						struct_del(p);				// �ش� ����ü ����
					}
					break;
				}
				else if(tmp[i]==tmp2[j])				connection_flag=1;
				else if(tmp[i]=='?')					connection_flag=1;
				// ���ų� ���ϵ�ī�� ? ��� ���ٰ� ����
				else{
					if(tmp[i]!=tmp2[j]){			// �� ���ڰ� �ٸ��ٸ�
						if(tmp[i]==' '||tmp[i]=='-')		i++;
						if(tmp2[j]==' '||tmp2[j]=='-')		j++;
						// ���� �����̰ų� '-' ���ڶ�� ���� ���ڷ� �̵�.
						if(tmp[i]==tmp2[j])			connection_flag=1;	// ������ ���ٰ� ����
						else						connection_flag=0;	// �ٸ��� �ٸ��ٰ� ����
					}
				}
				if(tmp[i]==0&&connection_flag==1){
					if (flag2 != 1){				// delete�� �ƴϸ�
						printf("name : %s, phone : %s\n", p->name, p->phone);
						write_person(p);				// �ش� ����ü ���Ͽ� �ۼ�
					}
					if (flag2 == 1){				// delete���
						printf("name : %s, phone : %s\n", p->name, p->phone);
						struct_del(p);				// �ش� ����ü ����
					}
					break;
				}
				else if(connection_flag==0)				break;
			}
			p=p->next;								// ���� ����ü�� �̵�
		}
	}
}
void struct_add(char *name, char* phone){			// �ش� ����ü ����
	Person *new;
	Person *p=person_head;							// �ӽó�� p�� head������ ����
	new = struct_reset(new);						// ����ü ����(�����Ҵ�Ϸ�)
	blank_clear(name);								// �̸��� �޴����� �������� ������ �����Ѵٸ� ���� ����
	blank_clear(phone);
	struct_data_save(new,0,name);					// 0�ϰ��, ������ ����ü�� �̸� ����
	struct_data_save(new,1,phone);					// 1�ϰ��, ������ ����ü�� �޴�����ȣ ����
	if(p==NULL){									// head�� �������, head�� ���� ������ ����ü�� ����
		person_head=new;
		return ;
	}
	while(1){
		if(p->next==LAST_NODE){						// p�� next��尡 �������, p�� next��带 ���� ������ ����ü�� ����
			p->next=new;
			break;
		}
		else	p=p->next;							
	}
}
void struct_del(Person *p){
	Person *pp = person_head;
	int flag=0;
	while(1){
		if(pp==p)	break;							// ��, �����Ϸ��� ��尡 head�ϰ��, break;(ã�ұ� ����)
		if(pp->next==p)	break;						// �����Ϸ��� ��尡 pp�� next�ϰ��, break(ã�ұ� ����)
		else		pp=pp->next;
	}
	if(p==person_head){								// �����Ϸ��� ��尡 head�� ���ٸ�
		person_head=p->next;							// head�� �����Ϸ��� ����� ������尡 �ȴ�.
		free(p);									// �����Ϸ��� ��� �����Ҵ� ����
		p=NULL;										// P�� null�� ���� ( �����ϰ� )
	}
	else{											// �����Ϸ��� ��尡 head�� �ƴ϶��
		pp->next=p->next;							// pp�� next���� p�� next��尡 �ȴ�.
		free(p);									// �����Ϸ��� ��� �����Ҵ� ����
		p=NULL;										// p�� null�� ���� ( �����ϰ� )
	}
}
char bar_change2(char p){
	return p+13;
}
char blank_change2(char p){
	return p-13;
}
char small_letter_change2(char p){					// �빮�ڸ� �ҹ��ڷ� ��ȯ(�ѹ���)
	return p+=32;
}
char big_letter_change2(char p){					// �ҹ��ڸ� �빮�ڷ� ��ȯ(�ѹ���)
	return p-=32;
}
void small_letter_change(char *p){					// �빮�ڸ� �ҹ��ڷ� ��ȯ��(������ü)
	int i=0;
	while(1){
		if(p[i]=='\0')			break;				// ���̶��, �׸�
		if(p[i]>='A'&&p[i]<='Z')	p[i]+=32;		// �빮�ڸ� �ҹ��ڷ� ����
		i++;
	}
}
int name_compare(char *p,char *pp){
	int i=0;
	char temp[20],temp2[20];
	strcpy(temp,p);									// �ش� ���ڵ��� temp�� ���� ����
	strcpy(temp2,pp);
	small_letter_change(temp);						// �ҹ��ڷ� ��� ����
	small_letter_change(temp2);						// �ҹ��ڷ� ��� ����
	return strcmp(temp,temp2);						// �����ϴٸ� 0, temp�� ������ ������ ���� �ڿ��ִٸ� 1
													// temp�� ������ ������ �տ� �ִٸ� -1
}
void write_person(Person *temp){					// �ش����� ����, temp������ �Է�
	FILE *write;
	write=fopen("output.txt","a+");					// output.txt ���� �д´� ( �� ���� �߰����� )
	if(temp==NULL)			return;					// temp �����Ͱ� �������� ������ return
	fprintf(write,"name : %-20s, ", temp->name);
	fprintf(write,"phone : %-16s\n",temp->phone);	// �ش� �����͸� ���Ͽ� ����
	fclose(write);									// ���� ���� ����
}
void write_file_add(char *name,char *phone){		// �Է� ���� ����, �ش� �����͸� �Է�
	FILE *write;
	write=fopen("input.txt","a+");					// input.txt ���� �д´� ( �� ���� �߰����� )
	fprintf(write,"%s ",name);
	fprintf(write,"%s\n",phone);					// �ش� �����͸� ���Ͽ� ����
	fclose(write);									// ���� ���� ����
}
void write_file(int flag){
	// flag==0	-> other routes
	// flag==1	-> del route
	Person *p=person_head;
	FILE *write;
	int i=0;
	if(p==NULL)				return ;				// �ش� �����Ͱ� �������� �ʴ´ٸ� return
	if(flag==0)				write=fopen("output.txt", "a+");	// output�� �ۼ��� �� ���
	else if(flag==1)		write=fopen("input.txt","w");		// input�� �ۼ��� �� ���

	if(p==LAST_NODE){								// �ش� �����Ͱ� �������̶��
		if(flag==0)		fprintf(write,"not output data\n");		// output data�� ���ٰ� �ۼ�
		fclose(write);											// �ش� output.txt ���� ����
		return;
	}
	while(1){										
		if(p==LAST_NODE)	break;
		else{
			if(flag==0){							// flag�� 0�ϰ��, output.txt���Ͽ� �ۼ�
				fprintf(write,"name : %-20s, ", p->name);
				fprintf(write,"phone : %-16s\n",p->phone);
			}
			else if(flag==1){						// flag�� 1�ϰ��, input.txt���Ͽ� �ۼ�
				fprintf(write,"%s ",p->name);
				fprintf(write,"%s\n",p->phone);
			}
			p=p->next;
		}
	}
	fclose(write);
}
void struct_sort(){									// ����ü �������� ( �̸� �� �������� )
	Person *p=person_head;							// �ӽó�� p ����
	Person *pp=person_head;							// �ӽó�� pp���� ( �����̸� ���ڸ� ������ �� )
	Person *temp,*temp2,temp3,*move,*move2;			
	/*	move2 == p�� ������, move == pp�� ������
		temp == pp�� next��, temp2 == p�� next��
	*/
	int result;
	move2=NULL;
	if(person_head==NULL)		return;				// head���� ���ٸ� ����
	if(person_head==LAST_NODE)	return;				// head���� ���������� ���ٸ� ����
	while(1){
		if(p->next==LAST_NODE)	break;			
		if(pp->next==LAST_NODE){					
			p=p->next;								
			pp=p->next;
			if(pp==LAST_NODE)	break;				
			if(move2==NULL)		move2=person_head;	
			else				move2=move2->next;	
		}
		else					pp=pp->next;		
		result=name_compare(p->name,pp->name);		// 
		if(result<=0)			continue;			// �̸����� ���ų� p->name�� ������������ �տ� �ִٸ� continue;
		else if(result>0){							// �̸����� p->name�� ������ ������ �ڿ� �ִٸ�
			if(p->next==pp){						
				temp=pp->next;						// p�� next���� pp�� next������ ����
				p->next=temp;						// pp�� next���� p�� ����
				pp->next=p;
			}
			else{
				if(pp->next==LAST_NODE)		temp=LAST_NODE;
				else						temp=pp->next;
													// temp�� pp�� next�� ( temp = pp->next )
				temp2=p->next;
				move=person_head;
				while(1){							// move�� next���� pp�� �ɶ����� �ݺ� ( move->next = pp )
					if(move->next==pp)	break;
					else			move=move->next;
				}
				pp->next=temp2;						// swap
				p->next=temp;
				move->next=p;
			}
			if(move2!=NULL)		move2->next=pp;		// p�� �������� move2�� null�ƴ϶��(p�� head�� �ƴ϶��), move2�� next�� pp�� ����
			if(p==person_head)	person_head=pp;		// p�� ���� head��� pp�� head�� ����
			temp=p;
			p=pp;
			pp=temp;
		}
	}
}
void struct_data_save(Person *a,int flag,char* temp){	// �ش� ����ü�� flag�� �°� �̸�, �޴�����ȣ ���� 
	if(flag==0)			strcpy(a->name,temp);
	else if(flag==1)	strcpy(a->phone,temp);
}
void blank_clear(char* temp){						// �������� \0�̸� ����, �׷����ʰ� ������ ���ڿ� ������ �����Ѵٸ� ���� ����
	int i=0;
	for(;;i++){
		if(temp[i]=='\0')	break;
		else if(temp[i]==' '&&temp[i+1]=='\0'){
			temp[i]='\0';
			break;
		}
	}
}
void struct_output(){								// ����ü�� ȭ�鿡 ���
	Person *p=person_head;
	printf("output start\n");
	if(p==NULL){									// ����ü�� �������� �ʴٸ�
		printf("not output data\n");
		return ;
	}
	if(p==LAST_NODE){								// ���������� head�� �����������
		printf("not output data\n");
		return;
	}
	while(1){										// ����ü ȭ�鿡 ���
		if(p==LAST_NODE)	break;
		else{
			printf("name : %s, phone : %s \n",p->name,p->phone);
			p=p->next;
		}
	}
}
void struct_clear(){								// ����ü ��� ����
	Person *move=person_head;
	Person *pp;										// move�� next ��� �ӽ�����
	if(move==NULL)			return;					// head�� null�̶�� return
	if(move==LAST_NODE){							// head�� ������ ����� return
		printf("not free\n");
		return ;
	}
	while(1){
		if(move->next==LAST_NODE){					// move�� next�� �������̶��
			free(move);								// �����Ҵ� ����
			move=NULL;
			move=LAST_NODE;
			person_head=move;						// head�� ���������� ����
			break;
		}
		else{
			pp=move->next;							
			free(move);								// �����Ҵ� ����
			move=NULL;
			move=pp;
		}
	}
}	
Person* struct_reset(Person *a){					// ����ü �⺻���� Ʋ ����� �� ( �����Ҵ�, ���� )
	a=(Person*)malloc(sizeof(Person));
	memset(a->name,0,20);
	memset(a->phone,0,20);
	a->next=LAST_NODE;
	return a;
}
void struct_input(){								// input.txt���� �о� ����ü�� ������ �ֱ�
	Person *p=person_head;
	Person *pp;
	FILE *read;
	char ch;
	int i,start,number_count;
	char temp[20];
	memset(temp,0,20);
	read=fopen("input.txt", "r");
	i=start=number_count=0;
	if (read == NULL){
		printf("file open error\n");
		exit(0);
	}
	while (1){
		ch = getc(read);
		if (ch == EOF)			break;
		if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){// �̸�
			if(start==0){							// ����ü�� ��������� ������ 0, ������� ���ĺ��ʹ� 1
				if(person_head==NULL){
					person_head=struct_reset(person_head);
					p=person_head;
					pp=p;
					start=1;
				}
				else{
					p=struct_reset(p);
					start=1;
				}
			}
			if(start!=1)	continue;
			temp[i]=ch;
			i++;
			continue;
		}
		else if(ch>='0'&&ch<='9'){					// �޴�����ȣ
			if(start==1){
				blank_clear(temp);
				struct_data_save(p,0,temp);			// �ϴ� �̸� ����.
				start=2;							// �̸� ���� ��, start flag 2�� ����.
				i=number_count=0;
				memset(temp,0,20);					// temp �ʱ�ȭ
			}
			if(number_count==11)	continue;		// 11�ڸ� ������ ���ڴ� �������� ����
			temp[i]=ch;
			i++;
			number_count++;
			continue;
		}
		if(ch==' '){								// ����
			temp[i]=ch;
			i++;
			continue;
		}
		else if(ch=='-' && start==2 ){				// �޴�����ȣ �Է����̸鼭 '-'���, 
			temp[i]=ch;
			i++;
			continue;
		}
		if(ch==10&&start==2){						// �����Է� ��, �ƽ�Ű�ڵ尪�� 10�� ���ڶ��
			blank_clear(temp);						// ��������
			struct_data_save(p,1,temp);				// ����ü�� �޴��� ��ȣ ����
			if(p!=person_head){
				pp->next=p;
				pp=pp->next;
			}
			memset(temp,0,20);
			i=start=number_count=0;
		}
	}
	fclose(read);
}
void argc_check(int argc,char* argv[]){				// argv üũ
	switch(argc){
		case 1:										// ������ 1����� �߸��� �Է�
			argc0_system(argv);
		default:									
			if(strcmp(argv[1],"-find")==0){			// -find ��ɾ���
				char temp[20];
				memset(temp,0,20);
				struct_input();						// input.txt ���� �о�, ����ü ����
				struct_sort();						// ����ü sort
				if(argc!=2){						// �̸��̳� �޴�����ȣ �Է��ߴٸ�, find�� ���� ã�´�.
					strcpy(temp,make_temp(argc,argv));
					struct_find(name_check(argv),temp,0);
				}
				else if(argc==2){					// -find ��ɾ �������� ���,
					write_file(0);					// output.txt���Ͽ� �� ����ü ���� ���
					struct_output();				// ȭ�鿡 ���
				}
				struct_clear();						// ����ü ����
				exit(0);							// ���α׷� ����
			}
			else if(strcmp(argv[1],"-add")==0){		// -add ��ɾ���
				char phone[20],name[20];
				memset(phone,0,20);
				memset(name,0,20);
				struct_input();
				if(argc>=4)	strcpy(name,make_add(argc,argv,phone));
				else{
					struct_clear();
					argc0_system(argv);
				}
				struct_add(name,phone);				// ����ü ����
				struct_sort();
				struct_output();
				write_file_add(name,phone);			
				write_file(0);
				struct_clear();
				exit(0);
			}
			else if(strcmp(argv[1],"-del")==0){		// -del ��ɾ���
				char temp[20];
				memset(temp,0,20);
				struct_input();
				printf("del state\n");
				if(argc!=2){						// �ش� ���� ���� ������ ����
					strcpy(temp,make_temp(argc,argv));
					struct_find(name_check(argv),temp,1);
					write_file(1);
					struct_sort();
					struct_output();
					write_file(0);
				}
				else if(argc==2){					// ��ü ����
					struct_clear();
					write_file(1);
					struct_output();
					write_file(0);
					exit(0);
				}
				struct_clear();
				exit(0);
			}
			else if(strcmp(argv[1],"--help")==0)	argc0_help(argv);		// help��ɾ��ϰ��,
			else 									argc0_system(argv);
	}
}
void argc0_system(char* argv[]){
	printf("%s : invalid option\n",argv[0]);	// �߸��� �ɼǿ� ���� ����.
	printf("Try '%s --help' for more information\n",argv[0]);
	exit(0);
}
void argc0_help(char* argv[]){					// help�� ���õ� ����
	printf("Usage: %s [OPTION] [...] [...]\n\n",argv[0]);
	printf("%7s, data add. \tex) %s [-add] [name_data] [phone_data]\n","-add",argv[0]);
	printf("\t\t\t -> ex) %s -add kimkyungmin 010-7212-0485\n",argv[0]);
	printf("%7s, data delete. \tex) %s [-del] [name] [name_data]\n","-del",argv[0]);
	printf("\t\t\tex) %s [-del] [phone] [phone_data]\n",argv[0]);
	printf("\t\t\t -> ex) %s -del name kimkyungmin\n",argv[0]);
	printf("\t\t\t -> ex) %s -del phone 010-7212-0485\n",argv[0]);
	printf("\t\t\t -> ex) %s -del      -> all delete\n",argv[0]);
	printf("%7s, data find.\tex) %s [-find] [name] [name_data]\n","-find",argv[0]);
	printf("\t\t\tex) %s [-find] [phone] [phone_data]\n",argv[0]);
	printf("\t\t\t -> ex) %s -find name kimkyungmin\n",argv[0]);
	printf("\t\t\t -> ex) %s -find phone 010-7212-0485\n",argv[0]);
	printf("\t\t\t -> ex) %s -find      -> all output\n",argv[0]);
	printf("\n\t\t\t* whildcard possible(using) *\n");	
	printf("\t * -> asterisk pattern character is matches zero or more characters.\n");
	printf("\t ? -> question mart pattern character is matches exactly one character.\n");
	printf("\t\t\t -> ex) %s -find name a*\n",argv[0]);
	printf("\t\t\t -> ex) %s -find phone 010*\n",argv[0]);
	printf("\t\t\t -> ex) %s -del name a*\n",argv[0]);
	printf("\t\t\t -> ex) %s -del phone 010*\n",argv[0]);
	printf("\t not use(only whildcard)!\n");
	printf("\t\t\t -> ex) %s -find name ?\n",argv[0]);
	printf("\t\t\t -> ex) %s -find name *\n",argv[0]);
	printf("\t\t\t -> ex) %s -find name ?*\n",argv[0]);
	exit(0);
}
