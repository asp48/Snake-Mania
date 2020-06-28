#include<iostream.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int gd=DETECT,gm,x,y,count,score;
char p[10];
FILE *fp;


//main controls
int U=72,L=75,R=77,D=80,bs=10,speed=5,bk=0,fd=4,fs=4,sf=14,sb=2,bl=9,br=631,bt=29,bb=461;
char color[16][12];


void save(int );
int options(int );
void initialize();
void colours();
void size();
void controls();
void settings();
void getline();
void colorset();


//mouse pgms
int initmouse();
void showmouseptr();
void hidemouseptr();
void getmousepos(int*,int*,int*);
union REGS i, o;



class body
{
    int left,top,bottom,right,midx,midy;
public:
    ~body() {}
    void copy(body &);
    void increment(int );
    void decrement(int );
    void draw();
    void facedraw();
    int check();
    int out();
    void erase();
    void set(int []);
    void store();
    int touch(body &);
};
class hscore
{
    int scr,length;
    char *name;
public:
    void set(char *s,int n)
    {
        length=strlen(s);
        name=new char[length+1];
        strcpy(name,s);
        scr=n;
    }
    void pname()
    {
        outtext(name);
    }
    void pscore()
    {
        char array[5];
        sprintf(array,"%d",scr);
        outtext(array);
    }
    void store()
    {
        fprintf(fp,"%s\n%d\n",name,scr);
    }
    void chkswap(hscore &x)
    {
        if(scr<x.scr)
        {
            int t=scr;
            scr=x.scr;
            x.scr=t;
            char *m;
            length=strlen(x.name);
            m=new char[length+1];
            strcpy(m,x.name);
            delete x.name;
            length=strlen(name);
            x.name=new char[length+1];
            strcpy(x.name,name);
            delete name;
            length=strlen(m);
            name=new char[length+1];
            strcpy(name,m);
        }
    }
};
body a[70];
void body::facedraw()
{
    setcolor(15);
    rectangle(left,top,right,bottom);
    setfillstyle(1,sf);
    floodfill(midx,midy,15);
}

void body::store()
{
    fprintf(fp," %d %d %d %d",left,top,right,bottom);
}
int body::touch(body & z)
{
    if((midx==z.midx)&&(midy==z.midy))
        return 0;
    else
        return 1;
}

void body::set(int b[4])
{
    left=b[0];
    top=b[1];
    right=b[2];
    bottom=b[3];
    midx=left+(int)((right-left)/2);
    midy=top+(int)((bottom-top)/2);
}
void body::copy(body &x)
{
    left=x.left;
    right=x.right;
    top=x.top;
    bottom=x.bottom;
    midx=x.midx;
    midy=x.midy;
}
void body::increment(int m)
{
    switch(m)
    {
    case 1:
        left+=bs;
        midx+=(int)(bs/2);
        break;
    case 2:
        top+=bs;
        midy+=(int)(bs/2);
        break;
    case 3:
        right+=bs;
        midx+=(int)(bs/2);
        break;
    case 4:
        bottom+=bs;
        midy+=(int)(bs/2);
        break;
    }
}
void body::decrement(int m)
{
    switch(m)
    {
    case 1:
        left-=bs;
        midx-=(int)(bs/2);
        break;
    case 2:
        top-=bs;
        midy-=(int)(bs/2);
        break;
    case 3:
        right-=bs;
        midx-=(int)(bs/2);
        break;
    case 4:
        bottom-=bs;
        midy-=(int)(bs/2);
        break;
    }
}
void body::draw()
{
    setcolor(15);
    rectangle(left,top,right,bottom);
    setfillstyle(1,sb);
    floodfill(midx,midy,15);
}
int body::check()
{
    if(((midx<x+6)&&(midx>x-6))&&((midy<y+6)&&(midy>y-6)))
    {
        randomize();
        x=(bl+fs+1)+rand()%(br-fs-bl-fs-1);
        y=(bt+fs+1)+rand()%(bb-fs-bt-fs-1);
        count++;
        score+=10;
        return 1;
    }
    return 0;
}
int body::out()
{
    if((left<=bl)||(right>=br)||(top<=bt)||(bottom>=bb))
        return 0;
    else
        return 1;
}
void body::erase()
{
    setcolor(bk);
    rectangle(left,top,right,bottom);
    setfillstyle(1,bk);
    floodfill(midx,midy,bk);
}
void start()
{   int i;
    setbkcolor(WHITE);
    setcolor(GREEN);
    settextstyle(0,0,6);
    moveto(50,220);
    outtext("SNAKE MANIA");
    delay(2000);
}
void menu()
{
    setbkcolor(WHITE);
    fflush(stdin);
    fflush(stdout);
    cleardevice();
    setcolor(BLUE);
    settextstyle(0,0,4);
    moveto((getmaxx()/2)-120,50);
    outtext("Main Menu");
    setcolor(RED);
    settextstyle(0,0,2);
    moveto(100,175);
    outtext("1.New Game");
    moveto(100,225);
    outtext("2.Load Game");
    moveto(100,275);
    outtext("3.High Score");
    moveto(100,325);
    outtext("4.Settings");
    moveto(100,375);
    outtext("5.Exit");
    moveto(100,425);
    outtext("Enter Your Choice:");
    return;
}
void hs()
{
    hscore x[3];
    int i=0,j,temp,sc,cnt=0;
    char str[10],msg[10];
    fp=fopen("SHS.TXT","r");
    while(!feof(fp))
    {
        if(fscanf(fp,"%s",str)>0)
        {   fgets(msg,strlen(str)+1,fp);
            fscanf(fp,"%d",&sc);
            x[i].set(str,sc);
            i++;
            cnt++;
        }
    }
    fclose(fp);
    x[i].set(p,score);
    cnt++;
    for(i=0; i<(cnt-1); i++)
        for(j=0; j<(cnt-i-1); j++)
            x[j].chkswap(x[j+1]);
    cleardevice();
    setbkcolor(BLACK);
    setcolor(RED);
    settextstyle(0,0,4);
    moveto(75,75);
    outtext("NAME");
    moveto(375,75);
    outtext("SCORE");
    setcolor(GREEN);
    settextstyle(0,0,3);
    fp=fopen("SHS.TXT","w");
    if(cnt>3)cnt=3;
    for(i=0; i<cnt; i++)
    {
        moveto(75,150+(i*75));
        x[i].pname();
        moveto(430,150+(i*75));
        x[i].pscore();
        x[i].store();

    }
    fclose(fp);
    getch();
    return;
}





void animate()
{
    int i;
    cleardevice();
    setbkcolor(0);
    for(i=0; i<(getmaxx()/2); i+=10)
    {
        cleardevice();
        setcolor(WHITE);
        circle((getmaxx()/2),(getmaxy()/2),(i/4));
        setfillstyle(1,WHITE);
        floodfill((getmaxx()/2),(getmaxy()/2),WHITE);
        circle(i,250,50);
        setfillstyle(1,RED);
        floodfill(i,250,WHITE);
        circle(getmaxx()-i,250,50);
        setfillstyle(1,GREEN);
        floodfill(getmaxx()-i,250,WHITE);
        delay(110);
    }
    setcolor(6);
    settextstyle(0,0,10);
    moveto(130,150);
    outtext("START");
    getch();
    return;
}
void newgame(int ,int );

int main()
{
    int choice,b[4],ch,prev,i,j;
    initgraph(&gd,&gm,"C:\\TC\\BGI");
    colorset();
    start();
    cleardevice();
    setbkcolor(BLUE);
    cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tPlayer Name: ";
    cin>>p;
    do
    {
        menu();
        choice=getch();
        switch(choice-'0')
        {
        case 1:
            cleardevice();
            animate();
            initialize();
            newgame(R,1);
            getch();
            break;
        case 2:
            fp=fopen("SSAVE.TXT","r");
            if((ch=getc(fp))==EOF)
            {
                cleardevice();
                setcolor(BLUE);
                settextstyle(0,0,3);
                moveto(150,250);
                outtext("NO SAVED GAMES");
                getch();
                break;
            }
            rewind(fp);
            fscanf(fp,"%d %d %d %d %d",&ch,&count,&score,&x,&y);
            for(i=0; i<=count; i++)
            {
                for(j=0; j<4; j++)
                    fscanf(fp,"%d",&b[j]);
                a[i].set(b);
            }
            fclose(fp);
            newgame(ch,0);
            getch();
            break;
        case 3:
            hs();
            break;
        case 4:
            settings();
            initgraph(&gd,&gm,"C:\\TC\\BGI");
            break;
        default:
            cleardevice();
            return 0;
        }
    } while(1);
}
void newgame(int ch,int f)
{
    int i,flag,prev,v=1,k=1;
    char array[5];
    prev=ch;
    cleardevice();
    setbkcolor(bk);
    setcolor(BLUE);
    rectangle(bl,bt,br,bb);
    setcolor(YELLOW);
    settextstyle(0,0,2);
    moveto(230,10);
    outtext("SCORE:");
    moveto(330,10);
    sprintf(array,"%d",score);
    outtext(array);
    setcolor(fd);
    circle(x,y,fs);
    setfillstyle(1,fd);
    floodfill(x,y,fd);
    if(!f)
    {
        a[1].facedraw();
        for(i=2; i<=count; i++)
        {
            a[i].draw();
        }
        getch();
        a[count].erase();
    }
    while(a[0].out()&&k)
    {
        flag=a[0].check();
        for(i=count; i>0; i--)
            a[i].copy(a[i-1]);
        prev=ch;
        if(kbhit())
        {
            ch=getch();
            if(ch==0)ch=getch();
        }
        if(((ch==U)&&(prev==D))||((ch==D)&&(prev==U))||((ch==R)&&(prev==L))||((ch==L)&&(prev==R)))
            ch=prev;
        if(ch==U)
        {
            a[0].decrement(2);
            a[0].decrement(4);
        }
        if(ch==L)
        {
            a[0].decrement(1);
            a[0].decrement(3);
        }
        if(ch==R)
        {
            a[0].increment(1);
            a[0].increment(3);
        }
        if(ch==D)
        {
            a[0].increment(2);
            a[0].increment(4);
        }
        if(ch==27)
        {
            v=options(prev);
            if(v==0)
            {
                cleardevice();
                setbkcolor(bk);
                setcolor(BLUE);
                rectangle(bl,bt,br,bb);
                setcolor(YELLOW);
                settextstyle(0,0,2);
                moveto(230,10);
                outtext("SCORE:");
                moveto(330,10);
                sprintf(array,"%d",score);
                outtext(array);
                a[1].facedraw();
                for(i=2; i<=count; i++)
                    a[i].draw();
                ch=prev;
                flag=2;
            }
            else
                return;
        }
        setcolor(fd);
        circle(x,y,fs);
        setfillstyle(1,fd);
        floodfill(x,y,fd);
        if(!v)
        {
            getch();
            v=1;
            a[count].erase();
            delay(20);
        }
        if(flag==0)
        {
            a[0].facedraw();
            a[1].draw();
            a[count].erase();
            if(count==1)
                a[0].facedraw();
            else
                a[count-1].draw();
            delay(300/speed);
        }
        else if(flag==1)
        {
            cleardevice();
            setbkcolor(bk);
            setcolor(BLUE);
            rectangle(bl,bt,br,bb);
            setcolor(YELLOW);
            settextstyle(0,0,2);
            moveto(230,10);
            outtext("SCORE:");
            moveto(330,10);
            sprintf(array,"%d",score);
            outtext(array);
            a[0].facedraw();
            for(i=1; i<count; i++)
                a[i].draw();
            delay(300/speed);
        }
        if(count>4)
        {
            k=1;
            i=3;
            while((k==1)&&(i<count))
            {
                k=a[0].touch(a[i]);
                i++;
            }
        }
    }
    getch();
    cleardevice();
    setbkcolor(WHITE);
    setcolor(RED);
    settextstyle(0,0,2);
    moveto(250,200);
    outtext("SCORE:");
    moveto(350,200);
    sprintf(array,"%d",score);
    outtext(array);
    if(!f)
    {
        fp=fopen("SSAVE.TXT","w");
        fclose(fp);
    }
}



int options(int dir)
{   int g,h,button,m,n;
    setcolor(BLUE);
    bar3d(190,180,430,328,10,2);
    setfillstyle(1,WHITE);
    floodfill(310,254,BLUE);
    floodfill(435,254,BLUE);
    floodfill(310,175,BLUE);
    setcolor(0);
    line(190,254,430,254);
    settextstyle(0,0,3);
    moveto(260,200);
    outtext("SAVE");
    moveto(260,295);
    outtext("QUIT");
    showmouseptr();
    while(1)
    {
        getmousepos(&button,&g,&h);
        delay(100);
        if(button==1)
        {
            hidemouseptr();
            m=g;
            n=h;
            if((m>190)&&(m<430))
            {
                if((n>180)&&(n<254))
                {
                    save(dir);
                    return 1;
                }
                else if((n>254)&&(n<328))
                {
                    cleardevice();
                    exit(0);
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
    }
}
void save(int dir)
{
    int i;
    fp=fopen("SSAVE.TXT","w");
    fprintf(fp,"%d %d %d %d %d",dir,count,score,x,y);
    for(i=0; i<=count; i++)
        a[i].store();
    fclose(fp);
    cleardevice();
    settextstyle(0,0,2);
    setcolor(WHITE);
    moveto(175,225);
    outtext("Saved Successfully");
    return;
}
int initmouse()
{
    i.x.ax = 0;
    int86(0X33,&i,&o);
    return ( o.x.ax );
}

void showmouseptr()
{
    i.x.ax = 1;
    int86(0X33,&i,&o);
}

void getmousepos(int *button, int *g, int *h)
{
    i.x.ax = 3;
    int86(0X33,&i,&o);

    *button = o.x.bx;
    *g = o.x.cx;
    *h = o.x.dx;
}
void hidemouseptr()
{
    i.x.ax = 2;             // to hide mouse
    int86(0X33,&i,&o);
}
void initialize()
{
    count=1;
    score=0;
    int b[4];
    b[0]=bl+1;
    b[1]=240;
    b[2]=bl+1+bs;
    b[3]=240+bs;
    a[0].set(b);
    randomize();
    x=(bl+fs+1)+rand()%(br-fs-bl-fs-1);
    y=(bt+fs+1)+rand()%(bb-fs-bt-fs-1);
}
void settings()
{
    int c;
    cout<<"\n\n\n\n\n\n\t\t\tLOADING.......";
    closegraph();
    do
    {
        clrscr();
        cout<<endl;
        getline();
        getline();
        cout<<"\n\t\t\t\tSETTINGS\n";
        getline();
        getline();
        cout<<"\n1.Colours\n";
        getline();
        cout<<"\n2.Size\n";
        getline();
        cout<<"\n3.Speed\n";
        getline();
        cout<<"\n4.Controls\n";
        getline();
        cout<<"\n5.Set to Default\n";
        getline();
        cout<<"\nEnter Your Choice    : ";
        c=getche();
        clrscr();
        switch(c)
        {
        case '1':
            colours();
            break;
        case '2':
            size();
            break;
        case '3':
            cout<<"\n\n\tEnter The Speed(1-10)  : ";
            cin>>speed;
            speed=1+(speed-1)%10;
            break;
        case '4':
            controls();
            break;
        case '5':
            U=72;
            L=75;
            R=77;
            D=80;
            bs=10;
            speed=5;
            bk=0;
            fd=4;
            fs=4;
            sf=14;
            sb=2;
            bl=9;
            br=631;
            bt=29;
            bb=461;
            getline();
            cout<<"\n\t\tSet To Default Successfully\n";
            getline();
            cout<<"\t1.SnakeBody   ->\t"<<color[sb]<<"\t"<<"7 .LEFT      ->\t"<<(char)L<<endl;
            getline();
            cout<<"\t2.SnakeFace   ->\t"<<color[sf]<<"\t"<<"8 .RIGHT     ->\t"<<(char)R<<endl;
            getline();
            cout<<"\t3.Food        ->\t"<<color[fd]<<"\t"<<"9 .BodySize  ->\t"<<bs<<"\n";
            getline();
            cout<<"\t4.Background  ->\t"<<color[bk]<<"\t"<<"10.FoodSize  ->\t"<<fs<<"\n";
            getline();
            cout<<"\t5.UP          ->\t"<<(char)U<<"\t\t"<<"11.Speed     ->\t"<<speed<<"\n";
            getline();
            cout<<"\t6.DOWN        ->\t"<<(char)D<<"\t\t"<<"12.BORDER    ->\t"<<bl<<","<<bt<<","<<br<<","<<bb<<"\n";
            getline();
            getch();
            break;
        case 27:
            return;
        }
    } while(1);
}
void colours()
{
    int c,i;
    do
    {
        clrscr();
        cout<<endl;
        getline();
        getline();
        cout<<"\n\t\t\t\tCOLOURS\n";
        getline();
        getline();
        cout<<"\n\t1.SnakeBody     ->\t"<<color[sb]<<endl;
        getline();
        cout<<"\n\t2.SnakeFace     ->\t"<<color[sf]<<endl;
        getline();
        cout<<"\n\t3.Food          ->\t"<<color[fd]<<endl;
        getline();
        cout<<"\n\t4.Background    ->\t"<<color[bk]<<endl;
        getline();
        cout<<"\nEnter Your Choice    : ";
        c=getche();
        clrscr();
        switch(c)
        {
        case '1':
            cout<<endl;
            for(i=0; i<15; i++)
            {
                getline();
                cout<<"\t"<<i<<" -> "<<color[i]<<"\t\t"<<i+1<<" -> "<<color[i+1]<<"\n";
                i++;
            }
            getline();
            cout<<"\nEnter Your Choice  :";
            cin>>sb;
            sb=sb%16;
            break;
        case '2':
            cout<<endl;
            for(i=0; i<15; i++)
            {
                getline();
                cout<<"\t"<<i<<" -> "<<color[i]<<"\t\t"<<i+1<<" -> "<<color[i+1]<<"\n";
                i++;
            }
            getline();
            cout<<"\nEnter Your Choice  :";
            cin>>sf;
            sf=sf%16;
            break;
        case '3':
            cout<<endl;
            for(i=0; i<15; i++)
            {
                getline();
                cout<<"\t"<<i<<" -> "<<color[i]<<"\t\t"<<i+1<<" -> "<<color[i+1]<<"\n";
                i++;
            }
            getline();
            cout<<"\nEnter Your Choice  :";
            cin>>fd;
            fd=fd%16;
            break;
        case '4':
            cout<<endl;
            for(i=0; i<15; i++)
            {
                getline();
                cout<<"\t"<<i<<" -> "<<color[i]<<"\t\t"<<i+1<<" -> "<<color[i+1]<<"\n";
                i++;
            }
            getline();
            cout<<"\nEnter Your Choice  :";
            cin>>bk;
            bk=bk%16;
            break;
        case 27:
            return;
        }
    } while(1);
}
void size()
{
    int c;
    do
    {
        clrscr();
        cout<<endl;
        getline();
        getline();
        cout<<"\n\t\t\t\tSIZE\n";
        getline();
        getline();
        cout<<"\n\t1.BODY     ->\t"<<bs<<"\n";
        getline();
        cout<<"\n\t2.FOOD     ->\t"<<fs<<"\n";
        getline();
        cout<<"\n\t3.BORDER   ->\t"<<bl<<" "<<bt<<" "<<br<<" "<<bb<<"\n";
        getline();
        cout<<"\nEnter Your Choice    : ";
        c=getche();
        clrscr();
        switch(c)
        {
        case '1':
            cout<<"\n\n\tEnter The Body Size(10-50)  : ";
            cin>>bs;
            break;
        case '2':
            cout<<"\n\n\tEnter The Food Size(4-10)  : ";
            cin>>fs;
            fs=4+(fs-4)%7;
            break;
        case '3':
            do
            {
                clrscr();
                cout<<endl;
                getline();
                getline();
                cout<<"\n\t\t\tBorder Size\n";
                getline();
                getline();
                cout<<"\n\t1.LEFT    ->\t"<<bl<<"\n";
                getline();
                cout<<"\n\t2.TOP     ->\t"<<bt<<"\n";
                getline();
                cout<<"\n\t3.RIGHT   ->\t"<<br<<"\n";
                getline();
                cout<<"\n\t4.BOTTOM  ->\t"<<bb<<"\n";
                getline();
                cout<<"\nEnter Your Choice    : ";
                c=getche();
                clrscr();
                switch(c)
                {
                case '1':
		    cout<<"\n\tLeft(0-250) = ";
		    cin>>bl;
		    break;
		case '2':
		    cout<<"\n\tTop(30-170) = ";
		    cin>>bt;
		    break;
		case '3':
		    cout<<"\n\tRight(390-639) = ";
		    cin>>br;
		    break;
		case '4':
		    cout<<"\n\tBottom(300-370) = ";
		    cin>>bb;
		    break;
		case 27:
		    return;
		}
	    } while(1);
        case 27:
            return;
        }
    } while(1);
}

void controls()
{
    int c;
    do
    {
        clrscr();
        cout<<endl;
        getline();
        getline();
        cout<<"\n\t\t\t\tCONTROLS\n";
        getline();
        getline();
        cout<<"\n\t1.UP     ->\t"<<(char)U<<"\n";
        getline();
        cout<<"\n\t2.DOWN   ->\t"<<(char)D<<"\n";
        getline();
        cout<<"\n\t3.LEFT   ->\t"<<(char)L<<"\n";
        getline();
        cout<<"\n\t4.RIGHT  ->\t"<<(char)R<<"\n";
        getline();
        cout<<"\nEnter Your Choice    : ";
        c=getche();
        clrscr();
        switch(c)
        {
        case '1':
            cout<<"\n\tPress The Key   : ";
            U=getche();
            if(U==0)U=getche();
            getch();
            break;
        case '2':
            cout<<"\n\tPress The Key   : ";
            D=getche();
            if(D==0)D=getche();
            getch();
            break;
        case '3':
            cout<<"\n\tPress The Key   : ";
            L=getche();
            if(L==0)L=getche();
            getch();
            break;
        case '4':
            cout<<"\n\tPress The Key   : ";
            R=getche();
            if(R==0)R=getche();
            getch();
            break;
        case 27:
            return;
        }
    } while(1);
}
void getline()
{
    for(int i=0; i<40; i++)
        cout<<"--";
}
void colorset()
{
    strcpy(color[0], "BLACK      ");
    strcpy(color[1], "BLUE       ");
    strcpy(color[2], "GREEN      ");
    strcpy(color[3], "CYAN       ");
    strcpy(color[4], "RED        ");
    strcpy(color[5], "MAGENTA    ");
    strcpy(color[6], "BROWN      ");
    strcpy(color[7], "LIGHT GRAY ");
    strcpy(color[8], "DARK GRAY  ");
    strcpy(color[9], "LIGHT BLUE ");
    strcpy(color[10],"LIGHT GREEN");
    strcpy(color[11],"LIGHT CYAN ");
    strcpy(color[12],"LIGHT RED  ");
    strcpy(color[13],"MAGENTA    ");
    strcpy(color[14],"YELLOW     ");
    strcpy(color[15],"WHITE      ");
}
