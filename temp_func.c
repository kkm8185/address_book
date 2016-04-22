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
#define LAST_NODE ((Person*)-1)							// 매크로정의
extern Person *person_head;								// 다른파일의 Person struct를 참조 설정.
void argc_check(int,char* argv[]);						// 입력값 확인
void small_letter_change(char *p);						// 대문자를 소문자로 전환
char small_letter_change2(char);						// 대문자를 소문자로 전환
char big_letter_change2(char);							// 소문자를 대문자로 전환
char bar_change2(char);
char blank_change2(char);
int name_compare(char *p,char *pp);						// 이름 비교 ( 소문자로 바꿔서 비교함 )
void write_file_add(char*,char*);						// input 파일에 데이터 추가
void write_file(int);									// 해당 파일을 열어, write ( 없으면 생성, 존재하면 추가작성 )
void write_person(Person *temp);						// 해당 파일을 열어, write ( 이름 및 휴대폰번호 )
void struct_sort();										// 정렬하기
void struct_data_save(Person *a,int flag,char* temp);	// 구조체에 데이터 삽입하기
void blank_clear(char* temp);							// 마지막 글자가 공백일경우, 공백 제거
void struct_output();									// 구조체 화면에 출력
void struct_clear();									// 구조체 삭제
Person* struct_reset(Person *a);						// 해당 구조체 재 할당
void struct_input();									// 해당 파일을 읽어, struct 할당
void struct_del(Person *a);								// 해당 구조체 삭제
void struct_find(int flag,char*,int);					// 해당 구조체 찾기
void struct_add(char *name, char* phone);				// 해당 구조체 삽입
void argc0_system(char* argv[]);						// 입력을 잘못하거나 한개도 없을경우 해당 내용 출력
int name_check(char* argv[]);							// argv 두번째 인자 name or phone 확인
char* make_temp(int argc,char* argv[]);					// argv에 입력된 데이터를 temp에 저장
void argc0_help(char* argv[]);							// argv 잘못 입력했을 때, 설명문 출력
char* make_add(int argc, char* argv[], char* temp);		// -add를 통해 삽입

int name_check(char* argv[]){
	if(strcmp(argv[2],"name")==0)		return 0;		// argv[2] 값이 name일 경우, 0
	else if(strcmp(argv[2],"phone")==0)	return 1;		// argv[2] 값이 phone일 경우, 1
	else					argc0_system(argv);			// 그렇지 않을경우에는 argc0_system 호출하여 잘못된 입력에 대한 내용 호출
}
// name or phone word check.
char* make_add(int argc,char* argv[],char* temp){		// argv에서 이름값만 저장
	char name[20];
	int i;
	memset(name,0,20);
	// name reset
	strcpy(name,argv[2]);								// 이름부분인 [2]를 일단 저장.
	for(i=3;;i++){
		if(argv[i][0]>='0'&&argv[i][0]<='9')	break;	// [i]에서 숫자가 발견된다면 반복문 그만.
		if(argv[i]==NULL){								// 숫자는 아닌데, [i]가 null일경우,
			struct_clear();								// 현 구조체 삭제
			argc0_system(argv);							// 잘못된 옵션에 대해 설명
		}
		strcat(name," ");								// 숫자도 아니고, null도 아니라면, " "를 통해 연결시켜준다.
		strcat(name,argv[i]);							// temp에 저장.

	}
	// not number ? save word. but number ? stop save word.
	if(argv[i]==NULL){									// 이름 저장완료, 그런데, 다음값이 존재하지 않다면?
		struct_clear();									// 현 구조체 삭제
		argc0_system(argv);								// 잘못된 옵션에 대한 설명
	}
	strcpy(temp,argv[i]);								// temp라는 공간에 휴대폰 번호 첫부분(혹은 모든부분) 저장.
	i++;
	for(;i<argc;i++){									// 반복을 통해 휴대폰 번호 모두 저장
		strcat(temp," ");
		strcat(temp,argv[i]);
	}
	// number ? save word.
	return name;										// name값 return
	// return name value.(temp is Reference)
}
char* make_temp(int argc,char* argv[]){
	char temp[20];
	int i;
	memset(temp,0,20);
	// temp reset
	if(argc<=3){										// argc 개수가 3개보다 작으면 잘못된 옵션
		struct_clear();
		argc0_system(argv);
	}
	if(argc>=4){										// argc 개수가 4개보다 많거나 같으면
		strcpy(temp,argv[3]);
		for(i=4;i<argc;i++){
			strcat(temp," ");							// 옵션 뒤의 이름 혹은 휴대폰 번호에 대한 데이터 저장.
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
	Person *p=person_head;					// head를 p로 임시지정
	int i=0,connection_flag=0,j=0;
	while(1){
		if(p==NULL){						// p가 NULL이면
			if(flag2==1)		printf("not delete data\n");	// del 옵션이라면 해당 문구 출력
			break;
		}
		strcpy(tmp,temp);					// temp값을 tmp에 저장
		// change tmp value reset
		if(flag==0){						// name에 관련된 것이라면 
			if(p==LAST_NODE)						break;		// p가 마지막 노드라면 종료
			for(i=0,j=0,connection_flag=0;;i++,j++){
				if(i==0)	strcpy(tmp2,p->name);				// tmp2에 해당 노드의 이름값 저장.
				if(tmp[i]=='*'){								// 와일드카드 문자(*)이 존재한다면,
					if(flag2!=1){								// delete옵션이 아니라면
						printf("name : %s, phone : %s\n",p->name,p->phone);	// 화면 출력
						write_person(p);						// 파일에 작성
					}
					if(flag2==1){								// delete 옵션이라면
						printf("name : %s, phone : %s\n",p->name,p->phone);	// 화면출력
						struct_del(p);							// 해당 구조체 삭제.
					}
					break;
				}
				// * -> all output
				else if(tmp[i]==tmp2[j])				connection_flag=1;
				// 찾는 결과값과 p의 이름과 비교하여 같으면 connection_flag 1을통해 같다라는 것을 인지
				else if(tmp[i]=='?'){
					connection_flag=1;
				}
				// 와일드카드 (?)문자면 connection_flag 1을통해 같다라는 것을 인지
				else{
					if(tmp[i]!=tmp2[j]){	// 같지않다면
						if(tmp[i]==' ')				i++;	// tmp에 공백이 있다면 다음문자로 이동
						if(tmp2[j]==' ')			j++;	// tmp2에 공백이 있다면 다음문자로 이동
						if(tmp[i]!=tmp2[j]){	// 바꿨는데도 같지 않다면
							if(tmp[i]>='A'&&tmp[i]<='Z')		tmp[i]=small_letter_change2(tmp[i]);// 대문자면 소문자로 변경
							else if(tmp[i]>='a'&&tmp[i]<='z')	tmp[i]=big_letter_change2(tmp[i]);	// 소문자면 대문자로 변경
						}
						if(tmp[i]==tmp2[j])			connection_flag=1;	// 같으면 connection_flag 1을 통해 같다라는 것을 인지
						else						connection_flag=0;	// 다르면 connection_flag 0을 통해 다르다라는 것을 인지
					}
				}
				if(tmp[i]==0&&connection_flag==1){	// 뒤의 글자가 더 없고, connection_flag가 1과 같다면 같은것이다.
					if(flag2!=1){					// delete 옵션이 아니라면
						printf("name : %s, phone : %s\n",p->name,p->phone);
						write_person(p);			// 해당 person 파일에 작성
					}
					if(flag2==1){					// delete 옵션이라면
						printf("name : %s, phone : %s\n",p->name,p->phone);
						struct_del(p);				// 해당 구조체에서 삭제
					}
					break;
				}
				else if(connection_flag==0)				break;
			}
			p=p->next;								// 다음 구조체로 이동
		}
		else if(flag==1){							// 휴대폰에 관련된 것이라면
			if(p==LAST_NODE)						break;
			for(i=0,j=0,connection_flag=0;;i++,j++){
				if(i==0)						strcpy(tmp2,p->phone);
				if(tmp[i]=='*'){					// 와일드 카드일경우에는
					if(flag2!=1){					// delete가 아니면
						printf("name : %s, phone : %s\n",p->name,p->phone);
						write_person(p);			// 해당 구조체 파일에 작성
					}
					if(flag2==1){					// delete라면
						printf("name : %s, phone : %s\n",p->name,p->phone);
						struct_del(p);				// 해당 구조체 삭제
					}
					break;
				}
				else if(tmp[i]==tmp2[j])				connection_flag=1;
				else if(tmp[i]=='?')					connection_flag=1;
				// 같거나 와일드카드 ? 라면 같다고 인지
				else{
					if(tmp[i]!=tmp2[j]){			// 두 글자가 다르다면
						if(tmp[i]==' '||tmp[i]=='-')		i++;
						if(tmp2[j]==' '||tmp2[j]=='-')		j++;
						// 만일 공백이거나 '-' 문자라면 다음 문자로 이동.
						if(tmp[i]==tmp2[j])			connection_flag=1;	// 같으면 같다고 인지
						else						connection_flag=0;	// 다르면 다르다고 인지
					}
				}
				if(tmp[i]==0&&connection_flag==1){
					if (flag2 != 1){				// delete가 아니면
						printf("name : %s, phone : %s\n", p->name, p->phone);
						write_person(p);				// 해당 구조체 파일에 작성
					}
					if (flag2 == 1){				// delete라면
						printf("name : %s, phone : %s\n", p->name, p->phone);
						struct_del(p);				// 해당 구조체 삭제
					}
					break;
				}
				else if(connection_flag==0)				break;
			}
			p=p->next;								// 다음 구조체로 이동
		}
	}
}
void struct_add(char *name, char* phone){			// 해당 구조체 삽입
	Person *new;
	Person *p=person_head;							// 임시노드 p를 head값으로 설정
	new = struct_reset(new);						// 구조체 생성(동적할당완료)
	blank_clear(name);								// 이름과 휴대폰의 마지막에 공백이 존재한다면 공백 삭제
	blank_clear(phone);
	struct_data_save(new,0,name);					// 0일경우, 생성한 구조체에 이름 삽입
	struct_data_save(new,1,phone);					// 1일경우, 생성한 구조체에 휴대폰번호 삽입
	if(p==NULL){									// head가 없을경우, head를 새로 생성한 구조체로 설정
		person_head=new;
		return ;
	}
	while(1){
		if(p->next==LAST_NODE){						// p의 next노드가 없을경우, p의 next노드를 새로 생성한 구조체로 설정
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
		if(pp==p)	break;							// 즉, 삭제하려는 노드가 head일경우, break;(찾았기 때문)
		if(pp->next==p)	break;						// 삭제하려는 노드가 pp의 next일경우, break(찾았기 때문)
		else		pp=pp->next;
	}
	if(p==person_head){								// 삭제하려는 노드가 head와 같다면
		person_head=p->next;							// head는 삭제하려는 노드의 다음노드가 된다.
		free(p);									// 삭제하려는 노드 동적할당 해제
		p=NULL;										// P는 null로 설정 ( 안전하게 )
	}
	else{											// 삭제하려는 노드가 head가 아니라면
		pp->next=p->next;							// pp의 next노드는 p의 next노드가 된다.
		free(p);									// 삭제하려는 노드 동적할당 해제
		p=NULL;										// p는 null로 설정 ( 안전하게 )
	}
}
char bar_change2(char p){
	return p+13;
}
char blank_change2(char p){
	return p-13;
}
char small_letter_change2(char p){					// 대문자를 소문자로 전환(한문자)
	return p+=32;
}
char big_letter_change2(char p){					// 소문자를 대문자로 전환(한문자)
	return p-=32;
}
void small_letter_change(char *p){					// 대문자를 소문자로 전환함(글자전체)
	int i=0;
	while(1){
		if(p[i]=='\0')			break;				// 끝이라면, 그만
		if(p[i]>='A'&&p[i]<='Z')	p[i]+=32;		// 대문자면 소문자로 변경
		i++;
	}
}
int name_compare(char *p,char *pp){
	int i=0;
	char temp[20],temp2[20];
	strcpy(temp,p);									// 해당 문자들을 temp에 각각 저장
	strcpy(temp2,pp);
	small_letter_change(temp);						// 소문자로 모두 변경
	small_letter_change(temp2);						// 소문자로 모두 변경
	return strcmp(temp,temp2);						// 동일하다면 0, temp가 사전적 순서로 볼때 뒤에있다면 1
													// temp가 사전적 순서로 앞에 있다면 -1
}
void write_person(Person *temp){					// 해당파일 열어, temp데이터 입력
	FILE *write;
	write=fopen("output.txt","a+");					// output.txt 파일 읽는다 ( 맨 끝에 추가삽입 )
	if(temp==NULL)			return;					// temp 데이터가 존재하지 않으면 return
	fprintf(write,"name : %-20s, ", temp->name);
	fprintf(write,"phone : %-16s\n",temp->phone);	// 해당 데이터를 파일에 쓰기
	fclose(write);									// 읽은 파일 종료
}
void write_file_add(char *name,char *phone){		// 입력 파일 열어, 해당 데이터를 입력
	FILE *write;
	write=fopen("input.txt","a+");					// input.txt 파일 읽는다 ( 맨 끝에 추가삽입 )
	fprintf(write,"%s ",name);
	fprintf(write,"%s\n",phone);					// 해당 데이터를 파일에 쓰기
	fclose(write);									// 읽은 파일 종료
}
void write_file(int flag){
	// flag==0	-> other routes
	// flag==1	-> del route
	Person *p=person_head;
	FILE *write;
	int i=0;
	if(p==NULL)				return ;				// 해당 데이터가 존재하지 않는다면 return
	if(flag==0)				write=fopen("output.txt", "a+");	// output에 작성할 때 사용
	else if(flag==1)		write=fopen("input.txt","w");		// input에 작성할 때 사용

	if(p==LAST_NODE){								// 해당 데이터가 마지막이라면
		if(flag==0)		fprintf(write,"not output data\n");		// output data가 없다고 작성
		fclose(write);											// 해당 output.txt 파일 종료
		return;
	}
	while(1){										
		if(p==LAST_NODE)	break;
		else{
			if(flag==0){							// flag가 0일경우, output.txt파일에 작성
				fprintf(write,"name : %-20s, ", p->name);
				fprintf(write,"phone : %-16s\n",p->phone);
			}
			else if(flag==1){						// flag가 1일경우, input.txt파일에 작성
				fprintf(write,"%s ",p->name);
				fprintf(write,"%s\n",p->phone);
			}
			p=p->next;
		}
	}
	fclose(write);
}
void struct_sort(){									// 구조체 내부정렬 ( 이름 순 오름차순 )
	Person *p=person_head;							// 임시노드 p 설정
	Person *pp=person_head;							// 임시노드 pp설정 ( 움직이며 글자를 비교해줄 값 )
	Person *temp,*temp2,temp3,*move,*move2;			
	/*	move2 == p의 이전값, move == pp의 이전값
		temp == pp의 next값, temp2 == p의 next값
	*/
	int result;
	move2=NULL;
	if(person_head==NULL)		return;				// head값이 없다면 리턴
	if(person_head==LAST_NODE)	return;				// head값이 마지막값과 같다면 리턴
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
		if(result<=0)			continue;			// 이름값이 같거나 p->name이 사전적순서로 앞에 있다면 continue;
		else if(result>0){							// 이름값이 p->name이 사전적 순서로 뒤에 있다면
			if(p->next==pp){						
				temp=pp->next;						// p의 next값을 pp의 next값으로 설정
				p->next=temp;						// pp의 next값을 p로 설정
				pp->next=p;
			}
			else{
				if(pp->next==LAST_NODE)		temp=LAST_NODE;
				else						temp=pp->next;
													// temp는 pp의 next값 ( temp = pp->next )
				temp2=p->next;
				move=person_head;
				while(1){							// move의 next값이 pp가 될때까지 반복 ( move->next = pp )
					if(move->next==pp)	break;
					else			move=move->next;
				}
				pp->next=temp2;						// swap
				p->next=temp;
				move->next=p;
			}
			if(move2!=NULL)		move2->next=pp;		// p의 이전값인 move2가 null아니라면(p가 head가 아니라면), move2의 next는 pp로 설정
			if(p==person_head)	person_head=pp;		// p의 값이 head라면 pp를 head로 설정
			temp=p;
			p=pp;
			pp=temp;
		}
	}
}
void struct_data_save(Person *a,int flag,char* temp){	// 해당 구조체에 flag에 맞게 이름, 휴대폰번호 저장 
	if(flag==0)			strcpy(a->name,temp);
	else if(flag==1)	strcpy(a->phone,temp);
}
void blank_clear(char* temp){						// 마지막이 \0이면 종료, 그렇지않고 마지막 문자에 공백이 존재한다면 공백 제거
	int i=0;
	for(;;i++){
		if(temp[i]=='\0')	break;
		else if(temp[i]==' '&&temp[i+1]=='\0'){
			temp[i]='\0';
			break;
		}
	}
}
void struct_output(){								// 구조체를 화면에 출력
	Person *p=person_head;
	printf("output start\n");
	if(p==NULL){									// 구조체가 존재하지 않다면
		printf("not output data\n");
		return ;
	}
	if(p==LAST_NODE){								// 존재하지만 head가 마지막노드라면
		printf("not output data\n");
		return;
	}
	while(1){										// 구조체 화면에 출력
		if(p==LAST_NODE)	break;
		else{
			printf("name : %s, phone : %s \n",p->name,p->phone);
			p=p->next;
		}
	}
}
void struct_clear(){								// 구조체 모두 삭제
	Person *move=person_head;
	Person *pp;										// move의 next 노드 임시저장
	if(move==NULL)			return;					// head가 null이라면 return
	if(move==LAST_NODE){							// head가 마지막 노드라면 return
		printf("not free\n");
		return ;
	}
	while(1){
		if(move->next==LAST_NODE){					// move의 next가 마지막이라면
			free(move);								// 동적할당 해제
			move=NULL;
			move=LAST_NODE;
			person_head=move;						// head를 마지막노드로 설정
			break;
		}
		else{
			pp=move->next;							
			free(move);								// 동적할당 해제
			move=NULL;
			move=pp;
		}
	}
}	
Person* struct_reset(Person *a){					// 구조체 기본적인 틀 만드는 곳 ( 동적할당, 리셋 )
	a=(Person*)malloc(sizeof(Person));
	memset(a->name,0,20);
	memset(a->phone,0,20);
	a->next=LAST_NODE;
	return a;
}
void struct_input(){								// input.txt파일 읽어 구조체에 데이터 넣기
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
		if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){// 이름
			if(start==0){							// 구조체가 만들어지지 않으면 0, 만들어진 이후부터는 1
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
		else if(ch>='0'&&ch<='9'){					// 휴대폰번호
			if(start==1){
				blank_clear(temp);
				struct_data_save(p,0,temp);			// 일단 이름 저장.
				start=2;							// 이름 저장 후, start flag 2로 설정.
				i=number_count=0;
				memset(temp,0,20);					// temp 초기화
			}
			if(number_count==11)	continue;		// 11자리 이후의 숫자는 저장하지 않음
			temp[i]=ch;
			i++;
			number_count++;
			continue;
		}
		if(ch==' '){								// 공백
			temp[i]=ch;
			i++;
			continue;
		}
		else if(ch=='-' && start==2 ){				// 휴대폰번호 입력중이면서 '-'라면, 
			temp[i]=ch;
			i++;
			continue;
		}
		if(ch==10&&start==2){						// 숫자입력 중, 아스키코드값이 10인 문자라면
			blank_clear(temp);						// 공백제거
			struct_data_save(p,1,temp);				// 구조체에 휴대폰 번호 저장
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
void argc_check(int argc,char* argv[]){				// argv 체크
	switch(argc){
		case 1:										// 개수가 1개라면 잘못된 입력
			argc0_system(argv);
		default:									
			if(strcmp(argv[1],"-find")==0){			// -find 명령어라면
				char temp[20];
				memset(temp,0,20);
				struct_input();						// input.txt 파일 읽어, 구조체 생성
				struct_sort();						// 구조체 sort
				if(argc!=2){						// 이름이나 휴대폰번호 입력했다면, find를 통해 찾는다.
					strcpy(temp,make_temp(argc,argv));
					struct_find(name_check(argv),temp,0);
				}
				else if(argc==2){					// -find 명령어만 기입했을 경우,
					write_file(0);					// output.txt파일에 현 구조체 내용 출력
					struct_output();				// 화면에 출력
				}
				struct_clear();						// 구조체 삭제
				exit(0);							// 프로그램 종료
			}
			else if(strcmp(argv[1],"-add")==0){		// -add 명령어라면
				char phone[20],name[20];
				memset(phone,0,20);
				memset(name,0,20);
				struct_input();
				if(argc>=4)	strcpy(name,make_add(argc,argv,phone));
				else{
					struct_clear();
					argc0_system(argv);
				}
				struct_add(name,phone);				// 구조체 삽입
				struct_sort();
				struct_output();
				write_file_add(name,phone);			
				write_file(0);
				struct_clear();
				exit(0);
			}
			else if(strcmp(argv[1],"-del")==0){		// -del 명령어라면
				char temp[20];
				memset(temp,0,20);
				struct_input();
				printf("del state\n");
				if(argc!=2){						// 해당 내용 관련 데이터 삭제
					strcpy(temp,make_temp(argc,argv));
					struct_find(name_check(argv),temp,1);
					write_file(1);
					struct_sort();
					struct_output();
					write_file(0);
				}
				else if(argc==2){					// 전체 삭제
					struct_clear();
					write_file(1);
					struct_output();
					write_file(0);
					exit(0);
				}
				struct_clear();
				exit(0);
			}
			else if(strcmp(argv[1],"--help")==0)	argc0_help(argv);		// help명령어일경우,
			else 									argc0_system(argv);
	}
}
void argc0_system(char* argv[]){
	printf("%s : invalid option\n",argv[0]);	// 잘못된 옵션에 대해 설명.
	printf("Try '%s --help' for more information\n",argv[0]);
	exit(0);
}
void argc0_help(char* argv[]){					// help에 관련된 내용
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
