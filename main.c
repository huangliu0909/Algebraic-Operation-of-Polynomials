#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<malloc.h>

typedef struct abc{
    double coef;
    int exp;
    struct abc *next;
}abc,*hl;

hl New(void)//创建新链表
{
    hl a=(hl)malloc(sizeof(abc));//给a申请一个结点的空间
    a->next=NULL;//初始化
    a->coef=0.0;//初始化
    a->exp=-1;//初始化
    return a;
}

char Compare (int a,int b)//比较a，b数值
{
    if(a>b)
        return '>';
    else if(a<b)
        return '<';
    else
        return '=';
}

hl Attach (double c, int e,hl d)//增加一个节点，数据域第一个为c，第二个为d
{
    hl x;
    x=New();//创建新链表
    x->coef=c;//数据域第一项赋值
    x->exp=e;//数据域第二项赋值
    d->next=x;//连接两个链表
    return x;
}

float Calculate(hl x,double xo)//计算多项式的值
{
    int i;
    double sum1=1,sum2=0;
    while(x!=NULL)
    {
        sum1=1;
        for(i=1; i<=(x->exp); i++)
        {
            sum1=xo*sum1;
        }
        sum2=sum1*(x->coef)+sum2;
        x=x->next;
    }
    return sum2;
}


hl Order(hl a)//使多项式中的各项按X的降幂排列,类似冒泡排序
{
    hl p,q;
    int i,j;
    for(p=a; p->next!=NULL; p=p->next)
    {
        for(q=a; q->next!=NULL; q=q->next)
        {
            if((q->exp)<(q->next->exp))
            {
                i=q->exp;
                j=q->coef;
                q->coef=q->next->coef;
                q->exp=q->next->exp;
                q->next->coef=j;
                q->next->exp=i;
            }
        }
    }
    for(p=a;p->next!=NULL; )//合并同类项
    {
        if((p->exp)==(p->next->exp))
        {
            q=p->next;
            p->coef=(p->coef)+(p->next->coef);
            p->next=(p->next->next);
            free(q);//释放
        }
        else
            p=p->next;
    }
    return a;
}


void Print(hl a)//打印多项式
{
    Order(a);
    double x;
    int y;
    while(a!=NULL)
    {
        x=a->coef;
        y=a->exp;
        a=a->next;
        printf("%lf X^%d ",x,y);
        if(a!=NULL)
        {
            if(a->coef>0)
                printf("+");
        }

    }
}

hl Add(hl a,hl b)//多项式加法
{
    hl r,c;
    int x;
    c=New();
    r=c;
    while((a!=NULL)&&(b!=NULL))//若a，b都没有结束
        switch(Compare (a->exp,b->exp))
        {
        case '=':
            x=(a->coef) + (b->coef);
            if(x!=0)
                r=Attach(x,a->exp,r);
            a=a->next;
            b=b->next;
            break;
        case '>':
            r=Attach(a->coef,a->exp,r);
            a=a->next;
            break;
        case '<':
            r=Attach(b->coef,b->exp,r);
            b=b->next;
            break;
        }
    while(a!=NULL)
    {
        r=Attach(a->coef,a->exp,r);//连接a的剩余部分
        a=a->next;
    }
    while(b!=NULL)
    {
        r=Attach(b->coef,b->exp,r);
        b=b->next;
    }
    r=c;
    c=c->next;
    free(r);
    return c;
}

hl Sub(hl a,hl b)//减法
{
    hl p,q,c;
    c=New();
    q=b;
    p=c;
    while(q!=NULL)
    {
        p=Attach (-(q->coef),q->exp,p);//系数取反
        q=q->next;
    }
    p=c;
    c=c->next;
    free(p);
    return Add(a,c);
}

hl Mul(hl a,hl b)//多项式乘法
{
    hl p,q,r,c,sum;
    sum=NULL;
    q=b;
    p=a;
    while(q!=NULL)
    {
        c=New();
        r=c;
        while(p!=NULL)
        {
            r=Attach ((p->coef)*(q->coef),(p->exp)+(q->exp),r);
            p=p->next;
        }
        r=c;
        c=c->next;
        free(r);
        sum=Add(sum,c);
        q=q->next;
        p=a;
    }
    return sum;
}


void Div(hl a,hl b)//多项式除法
{
   hl m = (hl)malloc(sizeof(abc));
   hl n = (hl)malloc(sizeof(abc));
   hl p = (hl)malloc(sizeof(abc));
   hl q = (hl)malloc(sizeof(abc));
   m = a;
   n = b;
   p->next = NULL;
   q->next = NULL;
   if(m->next->exp < n->next->exp)//如果第一项指数小于除数的第一项指数，直接输出m
        Print(m);
   else{
        while((m->exp >= n->exp)&&m&&n)
        {
            p->coef = (m->coef)/(n->coef);
            p->exp = m->exp - n->exp;
            q = Attach(p->coef,p->exp,q);
            Order(q);
            q->next = NULL;
            hl c = (hl)malloc(sizeof(abc));
            hl cc = c;
            c->next = NULL;
            while(n->next)
            {
                hl d = (hl)malloc(sizeof(abc));
                d->coef = (n->coef)*(p->coef);
                d->exp = n->exp+p->exp;
                c = Attach(d->coef,d->exp,c);
                Order(c);
                c->next = NULL;
                n = n->next;
            }
            Order(cc);
            n = b;
            Order(m);
            m = Sub(m,cc);
            Order(m);
        }
        printf("(");Print(b);printf(")");
        printf("*");
        printf("(");Print(q);printf(")");//商
        printf("+");
        printf("(");Print(m);printf(")");//余数
   }

}

int main()
{
    hl a1,a2,s1,s2;
    FILE *fp1,*fp2;
    fp1=fopen("1.txt","r");
    fp2=fopen("2.txt","r");
    a1=New();
    a2=New();
    s1=NULL;
    s2=NULL;
    hl head1=NULL;
    hl head2=NULL;
    head1=a1;//保存头地址
    head2=a2;
    while(!feof(fp1))//若文件结束则返回非零值，未结束则返回零
    {
        s1=(hl)malloc(sizeof(abc));
        fscanf(fp1,"%lf %d",&s1->coef,&s1->exp);
            s1->next=NULL;
        a1->next = s1;
        a1=a1->next;
    }
    a1=head1;
    fclose(fp1);
    a1=a1->next;

    while(!feof(fp2))//若文件结束则返回非零值，未结束则返回零
    {
        s2=(hl)malloc(sizeof(abc));
        fscanf(fp2,"%lf %d",&s2->coef,&s2->exp);
            s2->next=NULL;
        a2->next = s2;
        a2=a2->next;
    }
    a2=head2;
    fclose(fp2);
    a2=a2->next;

    Print(a1);printf("\n");
    Print(a2);printf("\n");

    printf("+ : ");
    Print(Add(a1,a2));printf("\n");
    printf("- : ");
    Print(Sub(a1,a2));printf("\n");
    printf("* : ");
    Print(Mul(a1,a2));printf("\n");
    printf("/ : ");
    Div(a1,a2);printf("\n");

    printf("Input an X: ");
    double x,a11,a22;
    scanf("%lf",&x);
    a11=Calculate(a1,x);
    a22=Calculate(a2,x);
    printf("a1+a2=%lf\n",a11+a22);
    printf("a1-a2=%lf\n",a11-a22);
    printf("a1*a2=%lf\n",a11*a22);
    printf("a1/a2=%lf\n",a11/a22);

    return 0;
}
