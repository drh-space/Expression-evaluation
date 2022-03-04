#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#define INITSIZE 100
#define cls system("cls")
#define pause system("pause")

//������ʽ�ṹ��
typedef struct{
    char a[1024];
    int result;
}exp;

//������ջ
typedef int ElemType;
typedef struct{
    ElemType *top;
    ElemType *base;
    int stacksize;
}numstack;

//�����ջ
typedef struct{
    char *top;
    char *base;
    int stacksize;
}signstack;

//��������
int Readfile(exp *bank, int i, int itemnum);
void Checkitem(int i, int allitem);
int Savescore(int *m, int num);
int Saveinput(int *m, int num);

//��ʼ��������ջ
void Initnum(numstack *S1){
    S1->base= (ElemType *)malloc(INITSIZE*sizeof(ElemType));
    if(!S1->base){
        printf("��ʼ��ʧ��initnum��\n\n");
        pause;
        exit(0);
    }
    S1->top= S1->base;
    S1->stacksize=INITSIZE;
}

//��ʼ�������ջ
void Initsign(signstack *S2){
    S2->base= (char *)malloc(INITSIZE*sizeof(char));
    if(!S2->base){
        printf("��ʼ��ʧ��initsign��\n\n");
        pause;
        exit(0);
    }
    S2->top= S2->base;
    S2->stacksize=INITSIZE;
}

//ȡ������ջ��Ԫ��
ElemType Getnum(numstack *S1){
    long e;
    if(S1->top== S1->base){
        printf("ջ��getnum��\n\n");
        pause;
        return 1;
    }
    e= *(S1->top-1);
    return e;
}

//ȡ�����ջ��Ԫ��
char Getsign(signstack *S2){
    char c;
    if(S2->top== S2->base){
        printf("ջ��getsign��\n\n");
        pause;
        return 1;
    }
    c= *(S2->top-1);
    return c;
}

//��������ջ
int Pushnum(numstack *S1, ElemType e){
    *S1->top++= e;
    return 0;
}

//�������ջ
int Pushsign(signstack *S2, char c){
    *S2->top++= c;
    return 0;
}

//��������ջ
int Popnum(numstack *S1){
    ElemType e;
    if(S1->top== S1->base){
        printf("ջ��popnum��\n\n");
        pause;
        return 1;
    }
    e= *--S1->top;
    return e;
}

//�������ջ
int Popsign(signstack *S2){
    char c;
     if(S2->top== S2->base){
        printf("ջ��popsign��\n\n");
        pause;
        return 1;
    }
    c= *--S2->top;
    return c;
}

//�ж��Ƿ��������
int Isoptr(char ch1){
    char ptr[10]= {'+', '-', '*', '/', '(', ')', '#'};
    for(int i=0; i<7; i++){
        if(ch1== ptr[i])
            return 1;
    }
    return 0;
}

//���ȼ����
char Precede(char a, char b){
    int i, j;
    char Table[10][10]={
    {' ','+','-','*','/','(',')','#'},
    {'+','>','>','<','<','<','>','>'},
    {'-','>','>','<','<','<','>','>'},
    {'*','>','>','>','>','<','>','>'},
    {'/','>','>','>','>','<','>','>'},
    {'(','<','<','<','<','<','=','0'},
    {')','>','>','>','>','0','>','>'},
    {'#','<','<','<','<','<','0','='}};

    for(i=0; i<8; i++)
        if(Table[0][i]== a) //����ջ������
            break;
    for(j=0; j<8; j++) //���ҵ�ǰ����
        if(Table[j][0]== b)
            break;
    return Table[j][i];
}

//����
int Operate(int a, char sign1, int b){
    int c;
    if(sign1== '+') c= a+b;
    else if(sign1== '-') c= a-b;
    else if(sign1== '*') c= a*b;
    else c= a/b;
    return c;
}

//��ֵ
long Calculate(char *ch, numstack *opnd, signstack *optr){
    char sign;
    int b, d, k=0, i=0, j=0, num= 0;
    Initnum(opnd);
    Initsign(optr);
    Pushsign(optr, '#');
    while(ch[i]!= '='){
        if(!Isoptr(ch[i])){
            k++;
            if(k<= j){
                num= (int(ch[i])-48);
                i++;
            }
            if(k> j){
                num= num*10+(int(ch[i])-48);
                k= j= 0;
                i++;
            }
            if(!Isoptr(ch[i]))
                k++;
            if(k==j)
                Pushnum(opnd, num);
            }
            else if(Isoptr(ch[i])){
                switch(Precede(ch[i], Getsign(optr))){
                    case'<': Pushsign(optr, ch[i]); i++;
                        if(ch[i]!= '(' && ch[i]!= ')')
                            j++;
                        break;
                    case'=': Popsign(optr); i++; break;
                    case'>': sign= Popsign(optr);
                        d= Popnum(opnd);
                        b= Popnum(opnd);
                        Pushnum(opnd, Operate(b, sign, d));
                        break;
                }
            }
    }
    printf("������ȷ���Ϊ:");
    printf("%d\n\n", Getnum(opnd));
    return(Getnum(opnd));
}

//(1)����������ļ�
int Creitembank(void){
    cls;
    int i, itemnum, select;
    exp *bank;
    FILE *fp;
    printf("*******************************************************\n");
    printf("*���½��������������1,��ԭ�����������������������2��\n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        fp=fopen("D:\\���ݽṹʵѵ\\�����.txt","wb");
    if(select== 2)
        fp=fopen("D:\\���ݽṹʵѵ\\�����.txt","ab");
    if(fp== NULL){
        printf("��û����������ļ�D:\\���ݽṹʵѵ\\�����.txt\n\n");
        pause;
        return 1;
    }
    printf("�����������������\n");
    scanf("%d",&itemnum);
    fflush(stdin);
    bank=(exp *)malloc(itemnum*2*sizeof(exp));   //��̬�����ڴ�
    if(bank==NULL){
        printf("�洢�ռ䲻�㡣\n\n");
        pause;
        return 1;
    }
    for(i=0; i<itemnum; i++){      //��������
        printf("������� %d ������:",i+1);
        gets(bank[i].a);
        fputs(bank[i].a, fp);
        fputs("\n",fp);
        fflush(stdin);
        printf("\n");
    }
    fclose(fp);
    printf("�ɹ������� %d �����⵽D:\\���ݽṹʵѵ\\�����.txt�ļ��С�\n\n", itemnum);
    pause;
    cls;
    Readfile(bank, i, itemnum);
    pause;
    free(bank);  //�ͷŶ�̬������ڴ�
    return 0;
}

//(2)�鿴�����������
void Checkitem(void){
    cls;
    FILE *fp;
    char str[1024];
    fp=fopen("D:\\���ݽṹʵѵ\\�����.txt","rb");
    if(fp== NULL){
        printf("�Ҳ���D:\\���ݽṹʵѵ\\�����.txt\n");
        pause;
        exit(0);
    }
    printf("��������е������б�:\n\n");
    while(!feof(fp)){
        fgets(str, 1024, fp);
        printf("%s", str);
        strcpy(str, " ");
    }
    fclose(fp);
    printf("\n");
    pause;
}

//(3)��ϰ����
int Practice(void){
    cls;
    FILE *fp, *mp;
    exp *extract;
    char reply, ans;
    char str[1024];
    int i=0, j, n, temp, item[20], result1;
    int score, count= 0, count1= 0, *grade;
    int total, select;
    numstack opnd1,*opnd;
    signstack optr1,*optr;
    opnd= &opnd1; optr= &optr1;
    grade=(int *)malloc(512*sizeof(int));
    extract=(exp *)malloc(512*sizeof(exp));
    do{
        cls;
        printf("������Ҫ��ϰ��ϰ����:\n");
        scanf("%d", &n);
        fp=fopen("D:\\���ݽṹʵѵ\\�����.txt", "rb");
        if(fp==NULL){
            printf("�Ҳ���D:\\���ݽṹʵѵ\\�����.txt\n");
            free(extract);
            exit(0);
        }
        while(!feof(fp) && i< n){
            fgets(extract[item[i]].a, 1024, fp);
        i++;
        }
        fclose(fp);
        score= 0;
        srand((unsigned)time(NULL));
        item[i]= rand()% n+1;
        while(i<= n){
            while(1){
                temp = rand()% n+1;
                for(j=0; j<i; j++){
                    if(item[j]== temp)
                        break;
                }
                if(j== i){
                    item[i]=temp;
                break;
                }
            }
            i++;
        }
        cls;
        printf("**************************************\n");
        printf("*�����ȡ�� %d ����ϰ��,�뿪ʼ����:   \n", n);
        printf("**************************************\n");
        pause;
        fflush(stdin);
        for(i= 0; i< n; i++){
            cls;
            count++;
            printf("�� %d ����ϰ��:\n\n",i+1);
            puts(extract[item[i]].a);
            printf("��������Ĵ�:\n");
            fflush(stdin);
            scanf("%d", &extract[item[i]].result);
            result1= extract[item[i]].result;
            printf("Ҫ��������Ĵ���?(Y/N)\n");
            fflush(stdin);
            ans= getchar();
            if(ans!= 'N' && ans!= 'n')
                Saveinput(&result1, count);
            if(extract[item[i]].result== Calculate(extract[item[i]].a, opnd, optr)){
                score+=10;
                printf("�ش���ȷ��\n\n");
            }
            else{
                printf("�ش����\n\n");
            }
            pause;
        }
        grade[count1]= score;
        count1++;       //��¼�������
        cls;
        printf(">>>>>�÷����<<<<<\n\n");
        printf(">>>>>���ĵ÷�Ϊ:%d��\n\n", score);
        total= n*10;
        if(score*1.0>= total*0.9)
            printf("�ɼ��ܰ���\n\n");
        else if(score*1.0>= total*0.8)
            printf("�ɼ����á�\n\n");
        else if(score*1.0>= total*0.7)
            printf("�ɼ�һ�㡣\n\n");
        else if(score*1.0>= total*0.6)
            printf("�ɼ�����\n\n");
        else
            printf("�ɼ�δ����Ҫ��������\n\n");
        printf("Ҫ����÷���?(Y/N)\n");
        fflush(stdin);
        ans= getchar();
        if(ans!= 'N' && ans!= 'n')
            Savescore(grade, count1);
        printf("������ϰ��?(Y/N)\n");
        fflush(stdin);
        reply= getchar();
        if(count1>= 20)    //��������¼������0
            count1= 0;
        }while(reply!='N' && reply!='n');
        free(grade); free(extract);
    pause;
    return count1;
}

//�����û�����Ĵ�
int Saveinput(int *m, int num){
    FILE *mp;
    int select;
    printf("*******************************************************\n");
    printf("*���±�������Ĵ�������1,��ԭ�������������2��\n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        mp= fopen("D:\\���ݽṹʵѵ\\����Ĵ�.txt", "wb");
    else if(select== 2)
        mp= fopen("D:\\���ݽṹʵѵ\\����Ĵ�.txt", "ab");
    else if(mp== NULL){
        printf("�Ҳ����÷ּ�¼�ļ�D:\\���ݽṹʵѵ\\����Ĵ�.txt\n\n");
        pause;
        return 1;
    }
    else{
        printf("��������Saveinput��\n\n");
        pause;
        return 1;
    }
    for(int i= 0; i< num; i++){
        fprintf(mp, "%d\n", m[i]);
    }
    fclose(mp);
    printf("�𰸱���ɹ���\n\n");
    pause;
    printf("\n");
    return 0;
}

//����÷ּ�¼
int Savescore(int *m, int num){
    FILE *mp;
    int select, i;
    printf("*******************************************************\n");
    printf("*���±���÷ּ�¼������1,��ԭ�е÷ּ�¼���������2�� \n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        mp= fopen("D:\\���ݽṹʵѵ\\�÷ּ�¼.txt", "wb");
    else if(select== 2)
        mp= fopen("D:\\���ݽṹʵѵ\\�÷ּ�¼.txt", "ab");
    else if(mp== NULL){
        printf("�Ҳ����÷ּ�¼�ļ�D:\\���ݽṹʵѵ\\�÷ּ�¼.txt\n\n");
        pause;
        return 1;
    }
    else{
        printf("��������Savescore��\n\n");
        pause;
        return 1;
    }
    for(i= 0; i< num; i++){
        fprintf(mp, "%d\n", m[i]);
    }
    fclose(mp);
    printf("�÷ֱ���ɹ���\n\n");
    pause;
    printf("\n");
    return 0;
}

//(4)�鿴�û�����Ĵ�
int Checkinput(int num){
    cls;
    char m[50];
    printf("֮ǰ����Ĵ�:\n\n");
    FILE *mp;
    mp=fopen("D:\\���ݽṹʵѵ\\����Ĵ�.txt","r");
     if(mp==NULL){
        printf("�Ҳ�������Ĵ��ļ�D:\\���ݽṹʵѵ\\����Ĵ�.txt\n\n");
        pause;
        return 1;
    }
    for(int i= 0; i< num; i++){
        fgets(&m[i], num, mp);
        printf("*�� %d ������Ĵ�:\n",i+1);
        printf("%d\n\n", m[i]);
    }
    fclose(mp);
    pause;
    return 0;
}

//(5)�鿴�÷ּ�¼
int Checkscore(int num){
    cls;
    FILE *mp;
    char m[50];
    mp=fopen("D:\\���ݽṹʵѵ\\�÷ּ�¼.txt","r");
    if(mp==NULL){
        printf("�Ҳ���D:\\���ݽṹʵѵ\\�÷ּ�¼.txt\n\n");
        pause;
        return 1;
    }
    printf(">>>>>�÷����<<<<<��\n\n");
    fgets(m, num, mp);
    for(int i= 0; i< num; i++){
        printf("�� %d �ε÷�:\n",i+1);
        printf("%d\n\n", m[i]);
    }
    fclose(mp);
    pause;
    return 0;
}

//(6)���յ÷ָ�������
void Evaluate(int count1){
    cls;
    int *grade;
    int i, total;
    total= count1*10;
    grade=(int *)malloc(sizeof(int));
    Checkscore(count1);
    printf("\n");
    cls;
    printf(">>>>>СС����<<<<<\n\n");
    for(i=0; i< count1; i++)
        printf("�� %d �ε÷� %d��\n", i+1, grade[i]);
    if(i> 1){
        if(grade[i-1]> total*0.6 || grade[i-2]> total*0.6){
            if(grade[i-1]> grade[i-2])
                printf("\n�н�����ҲҪ��������\n\n");
            else if(grade[i-1]== grade[i-2])
                printf("\n���ϴβ�࣬Ҫ��ȡ��������������\n\n");
            else
                printf("\n�˲��˰����Ͽ첹��������������\n\n");
        }
        else
            printf("\n���⣬�˺��˺ܶࡣ��������\n\n");
    }
    else{
        if(grade[0]*1.0>= total*0.9)
            printf("�ɼ��ܰ���\n\n");
        else if(grade[0]*1.0>= total*0.8)
            printf("�ɼ����á�\n\n");
        else if(grade[0]*1.0>= total*0.7)
            printf("�ɼ�һ�㡣\n\n");
        else if(grade[0]*1.0>= total*0.6)
            printf("�ɼ�����\n\n");
        else
            printf("�ɼ�δ����Ҫ��������\n\n");
    }
    free(grade);
    pause;
}

//��ȡ�����������
int Readfile(exp *bank, int i, int itemnum){
    FILE *fp;
    fp=fopen("D:\\���ݽṹʵѵ\\�����.txt","rb");
    if(fp==NULL){
        printf("�Ҳ���D:\\���ݽṹʵѵ\\�����.txt\n\n");
        pause;
        exit(0);
    }
    fgets(bank[i].a, itemnum, fp);
    printf("�ոս����������б�\n\n");
    for(i=0; i<itemnum;i++){     //�������
        printf("*�� %d ������:",i+1);
        printf("%s",bank[i].a);
        printf("\n\n");
    }
    fclose(fp);
    return 0;
}

void Menu(){
    system("cls");
	printf("****************************************************************\n");
	printf("           ���ʽ��ֵ���򣬲��ܸ����������ɹ�Сѧ����ҵ��ϰ   \n");
	printf("****************************************************************\n\n");
	printf("           1.д�����⡣\n\n");
	printf("           2.�鿴���⡣\n\n");
	printf("           3.��ϰ���⡣\n\n");
	printf("           4.�鿴����𰸡�\n\n");
	printf("           5.�鿴�÷������\n\n");
	printf("           6.���۵÷������\n\n");
	printf("           7.�˳�����\n\n");
	printf("****************************************************************\n");
}

int main(){
    int m, select, ret, num;
    do{
        Menu();
        printf("��ѡ��������Ž�����Ӧ����:\n");
        ret=scanf("%d", &select);
        switch(select){
            case 1: Creitembank(); break;
            case 2: Checkitem(); break;
            case 3: m=Practice(); break;
            case 4: cls; printf("����Ҫ�鿴�ļ�¼����(��):\n"); scanf("%d", &num); Checkinput(num); break;
            case 5: cls; printf("����Ҫ�鿴�ļ�¼����(����):\n"); scanf("%d", &num); Checkscore(num); break;
            case 6: cls; printf("����Ҫ�鿴�ļ�¼����(����):\n"); scanf("%d", &num); Evaluate(num); break;
            case 7: cls; printf("*���������*\n"); exit(0);
            default:
                printf("*************************************************\n");
                printf("* *������������ȷѡ����ţ�1-7��������Ӧ����* *��\n");
                printf("*************************************************\n");
                pause;
                break;
        }
    }while(ret==1);
    return 0;
}
