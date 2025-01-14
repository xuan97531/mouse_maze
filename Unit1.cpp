//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define possible_direction 8
#define S 4
TForm1 *Form1;

int m,n,x,y,top=-1,d,i,j;
int starti=1,startj=0,endi,endj,end_i,end_j,success;
int **maze;
int **mark;
int B[200][200];
int **A;
int **w;
struct position*Stack;

struct offset{
	int dx,dy;
};
//enum directions {N,NE,E,SE,S,SW,W,NW};
struct offset move[possible_direction];
struct position{
	 int x,y;
	 int dir;
};
struct cood{
	 int dx,dy;
};
struct position next,stack[200];
struct cood f1[S];
struct cood f2[S];



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	 Memo1->Clear();
	 move[0].dx = -1;   move[0].dy = 0;
	 move[1].dx = -1;  move[1].dy = 1;
	 move[2].dx = 0;    move[2].dy = 1;
	 move[3].dx = 1;   move[3].dy = 1;
	 move[4].dx = 1;    move[4].dy = 0;
	 move[5].dx = 1;   move[5].dy = -1;
	 move[6].dx = 0;    move[6].dy = -1;
	 move[7].dx = -1;  move[7].dy = -1;

}
//---------------------------------------------------------------------------

void push(struct position data){
	 if(top==(m*n-1));
	 else{
		Stack[++top] = data;
	 }
	 int d = data.dir;
	 Form1->Memo3->Lines->Add("push -> " + IntToStr(data.x) + "," + IntToStr(data.y) + " dir -> " + d + " top -> " + top);
}
struct position pop(){
	 if(top==-1);
	 else{
		Form1->Memo3->Lines->Add("push -> " + IntToStr(Stack[top].x) + "," + IntToStr(Stack[top].y) + " dir -> " + IntToStr(Stack[top].dir) + " top -> " + top);
		return Stack[top--];
	 }
}

void initation(){
	 f1[0].dx = 0; f1[0].dy = 1;
	 f1[1].dx =-1; f1[1].dy = 0;
	 f1[2].dx = 0; f1[2].dy =-1;
	 f1[3].dx = 1; f1[3].dy = 0;
	 f2[0].dx = 0; f2[0].dy = 2;
	 f2[1].dx =-2; f2[1].dy = 0;
	 f2[2].dx = 0; f2[2].dy =-2;
	 f2[3].dx = 2; f2[3].dy = 0;

	 for(i=0;i<S;i++)
		 for(j=0;j<S;j++) w[i][j] = 1;
}

int NextMove(struct position step){
	for(i=0;i<S;i++)
		if(w[step.x+f2[i].dx][step.y+f2[i].dy]==1) return 1;
	return 0;
}

/*int**MazeDFS(int**w,struct position step){
	 step.x = 1;step.y = 1;  step.dir = 0;
	 push(step);
	 w[step.x][step.y]=0;
	 while(top!=-1){
		step = pop();
		while(NextMove(step)){
		  d = rand()%S;
		  next.x = step.x+f2[d].dx;
		  next.y = step.y+f2[d].dy;
		  if(w[next.x][next.y]==1){
			w[step.x+f1[d].dx][step.y+f1[d].dy] = 0;
			push(next);
			w[next.x][next.y] = 0;
			step = next;
		  }
		}
	 }
	 return  w;
}   */

int **generatemaze(int ** A,int x,int y){
	 int direction;
	 A[x][y]=0;
	 while((A[x][y+1]!=2 && A[x][y+2]==1) ||(A[x+1][y]!=2 && A[x+2][y]==1) ||(A[x][y-1]!=2 && A[x][y-2]==1) ||(A[x-1][y]!=2 && A[x-2][y]==1)){
			direction = rand()%4;
			if(direction==0 && A[x][y+1]!=2 && A[x][y+2]==1){
			  A[x][y+1] = 0;
			  Form1->Memo4->Lines->Add("(x,y,dir)= "+IntToStr(x) + "," +IntToStr(y) + "," + IntToStr(direction));
			  A = generatemaze(A,x,y+2);
			}
			else if(direction==1 && A[x-1][y]!=2 && A[x-2][y]==1){
			  A[x-1][y] = 0;
			  Form1->Memo4->Lines->Add("(x,y,dir)= "+IntToStr(x) + "," +IntToStr(y) + "," + IntToStr(direction));
			  A = generatemaze(A,x-2,y);
			}
			else if(direction==2 && A[x][y-1]!=2 && A[x][y-2]==1){
			  A[x][y-1] = 0;
			  Form1->Memo4->Lines->Add("(x,y,dir)= "+IntToStr(x) + "," +IntToStr(y) + "," + IntToStr(direction));
			  A = generatemaze(A,x,y-2);
			}
			else if(direction==3 && A[x+1][y]!=2 && A[x+2][y]==1){
			   A[x+1][y] = 0;
			   Form1->Memo4->Lines->Add("(x,y,dir)= "+IntToStr(x) + "," +IntToStr(y) + "," + IntToStr(direction));
			   A = generatemaze(A,x+2,y);
			}
	 }
	 return A;
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{

	 AnsiString out;
	 if(StrToInt(Edit1->Text)%2==0||StrToInt(Edit2->Text)%2==0)  ShowMessage("�п�J�_��!!!");
	 else{
	 m = StrToInt(Edit1->Text);
	 n = StrToInt(Edit2->Text);
	 StringGrid3->RowCount = m;
	 StringGrid3->ColCount = n;
	 for (i=0; i<n; i++) StringGrid3->ColWidths[i] = 24;
	 for (i=0; i<m; i++) StringGrid3->RowHeights[i] = 24;
	 StringGrid3->FixedCols= 0;
	 StringGrid3->FixedRows= 0;
	 A = new int * [m];
	 for(i=0;i<m;i++) A[i] = new int [n];
	 int start_i=1,start_j=1,end_i=m-2,end_j=n-2;
	 int x,y;
	 for(i=0;i<m;i++)
		 for(j=0;j<n;j++){
			 if(j==0||j==n-1) A[i][j]=2;
			 else if(i==0||i==m-1) A[i][j]=2;
			 else A[i][j]=1;
		 }
	 A = generatemaze(A,end_i,end_j);
	 A[start_i][start_j-1]=0;
	 A[end_i][end_j+1]=0;
	 for(i=0;i<m;i++)
		 for(j=0;j<n;j++)
			 StringGrid3->Cells[j][i] = A[i][j];
	 for(i=0;i<m;i++){
			out="";
			for(j=0;j<n;j++){
				out += "  "+IntToStr(A[i][j]);
			}
			Memo3->Lines->Add(out);
		}
	 }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	 FILE *fp;
	 AnsiString out,fname;
	 int i,j;
	 if(OpenDialog1->Execute()){
		fname = OpenDialog1->FileName;
		fp = fopen(fname.c_str(),"r+");
		fscanf(fp,"%d %d",&m,&n);
        maze = new int *[m];
		for(i=0;i<m;i++) maze[i] = new int [n];
		mark = new int *[m];
		for(i=0;i<m;i++) mark[i] = new int [n];
		endi = m,endj = n;
		Memo1->Lines->Add("m = "+IntToStr(m));
		Memo1->Lines->Add("n = "+IntToStr(n));
		for(i=0;i<m;i++)
			for(j=0;j<n;j++){
				fscanf(fp,"%d",&maze[i][j]);
				mark[i][j]=maze[i][j];
			}
		StringGrid1->RowCount = m;
		StringGrid1->ColCount = n;
		StringGrid2->RowCount = m;
		StringGrid2->ColCount = n;
		StringGrid1->FixedCols= 0;
		StringGrid1->FixedRows= 0;

		for (i=0; i<n; i++) StringGrid1->ColWidths[i] = 24;
		for (i=0; i<m; i++) StringGrid1->RowHeights[i] = 24;
		StringGrid2->FixedCols= 0;
		StringGrid2->FixedRows= 0;

		for (i=0; i<n; i++) StringGrid2->ColWidths[i] = 24;
		for (i=0; i<m; i++) StringGrid2->RowHeights[i] = 24;
		Memo1->Lines->Add("----------Maze-----------");
		for(i=0;i<m;i++){
			out="";
			for(j=0;j<n;j++){
				out += "  "+IntToStr(maze[i][j]);
			}
			Memo1->Lines->Add(out);
		}
		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				StringGrid1->Cells[j][i] = maze[i][j];
				StringGrid2->Cells[j][i] = maze[i][j];
			}
		}
	 }
	 else Memo1->Lines->Add("Nothing Happens!");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	AnsiString text = StringGrid1->Cells[ACol][ARow];
	if (text == "0")
		StringGrid1->Canvas->Brush->Color = TColor RGB(228, 216, 220);
	else if(text == "1")
		StringGrid1->Canvas->Brush->Color = TColor RGB(201, 204, 213);
	else if(text == "2")
		StringGrid1->Canvas->Brush->Color = TColor RGB(147, 181, 198);
	else if(text == "3")
		StringGrid1->Canvas->Brush->Color = TColor RGB(255, 227, 227);
	else if(text == "4")
		StringGrid1->Canvas->Brush->Color = TColor RGB(201, 150, 204);
		StringGrid1->Canvas->FillRect(Rect);
		StringGrid1->Canvas->TextRect(Rect, Rect.Left+6, Rect.Top+8, text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{    Form1->DoubleBuffered = true;
	 int u,v,d;
	 bool found = false;
	 AnsiString out;
	 m = StrToInt(StringGrid2->RowCount);
	 n = StrToInt(StringGrid2->ColCount);
	 Stack = new struct position[m*n];
	 struct position step;
	 step.x = 1;step.y = 0;step.dir = 0;
	 push(step);
	 while(top != -1){
		step = pop();
		while((step.dir<=7)&&(!found)){
			i = step.x; j = step.y; d = step.dir;
			u = i + move[d].dx; v = j + move[d].dy;
			if((u == m-2) && (v == n)){
				found = true;
				while(top != -1){
					step = pop();
					mark[step.x][step.y] = 4;
					StringGrid1->Cells[step.y][step.x] = mark[step.x][step.y];
				}
			return;
			}
			else if(!mark[u][v]){
					mark[u][v] = 4;
					step.dir = step.dir+=1;
					push(step);
					step.x = u; step.y = v; step.dir = 0;
					StringGrid1->Refresh();
					step.x = u; step.y = v; step.dir = 0 ;
			}
			else step.dir+=1;
			mark[i][j] = 5;
		}
	 }
	 if(found){
        Memo1->Lines->Add("----------Mark-----------");
			mark[m-1][n]=3;
			for(i=0;i<m;i++){
				out="";
				for(j=0;j<n;j++){
					out += "  "+IntToStr(mark[i][j]);
				}
			Memo1->Lines->Add(out);
			}
		   for(i=0;i<m;i++){
			   for(j=0;j<n;j++){
					StringGrid1->Cells[j][i] = mark[i][j];
				}
		   }
	 }
     else Memo1->Lines->Add("No Exit !!!!");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	 if(SaveDialog1->Execute()){
		Memo1->Lines->SaveToFile(SaveDialog1->FileName);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{   int u,v;
	 m = StrToInt(Edit1->Text);
	 n = StrToInt(Edit2->Text);
	 Stack = new struct position[m*n];
	 struct position step;
	 AnsiString out;
	 w = new int * [m];
	 for(i=0;i<m;i++) w[i] = new int [n];
	 for(i=0;i<m;i++)
		 for(j=0;j<n;j++){
			 if(j==0||j==n-1) w[i][j]=2;
			 else if(i==0||i==m-1) w[i][j]=2;
			 else w[i][j]=1;
		 }
	 step.x = 1; step.y = 1; step.dir = 3;
	 push(step);
	 w[step.x][step.y]=0;
	 while(top!=-1){
		step = pop();
		i = step.x; j = step.y; d = step.dir;
		while(d <= 3){
		  u = i + f1[d].dx; v = j + f1[d].dy;
		  if((u == m-2) && (v == n)){
		  while(NextMove(step)){
			d = rand()%S;
			next.x = step.x+f2[d].dx;
			next.y = step.y+f2[d].dy;
			next.dir = d;
			if(w[next.x][next.y]==1){
			  w[step.x+f1[d].dx][step.y+f1[d].dy] = 0;
			  push(next);
			  w[next.x][next.y] = 0;
			  step = next;
			}
		  }
		  }
		}

	 }
      for(i=0;i<m;i++){
			out="";
			for(j=0;j<n;j++){
				out += "  "+IntToStr(A[i][j]);
			}
			Memo5->Lines->Add(out);
		}




}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
		AnsiString text = StringGrid2->Cells[ACol][ARow];
		if (text == "0")
			StringGrid2->Canvas->Brush->Color = TColor RGB(228, 205, 167);
		else if(text == "1")
			StringGrid2->Canvas->Brush->Color = TColor RGB(195, 176, 145);
		else if(text == "2")
			StringGrid2->Canvas->Brush->Color = TColor RGB(142, 128, 106);
		else if(text == "3")
			StringGrid2->Canvas->Brush->Color = TColor RGB(255, 227, 227);
		else if(text == "4")
			StringGrid2->Canvas->Brush->Color = TColor RGB(255, 230, 188);
		else if(text == "5")
			StringGrid2->Canvas->Brush->Color = TColor RGB(229, 220, 195);
		StringGrid2->Canvas->FillRect(Rect);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	 int u,v,d;
	 bool found = false;
	 AnsiString out;
	 m = StrToInt(StringGrid2->RowCount);
	 n = StrToInt(StringGrid2->ColCount);
	 Stack = new struct position[m*n];
	 struct position step;
	 step.x = 1;step.y = 0;step.dir = 0;
	 push(step);
	 while(top != -1){
		step = pop();
		while((step.dir<=7)&&(!found)){
			i = step.x; j = step.y; d = step.dir;
			u = i + move[d].dx; v = j + move[d].dy;
			if((u == m-2) && (v == n)){
				found = true;
				while(top != -1){
					Sleep(TrackBar1->Position);
					step = pop();
					maze[step.x][step.y] = 4;
					StringGrid2->Cells[step.y][step.x] = maze[step.x][step.y];
					StringGrid2->Refresh();
				}
			return;
			}
			else if(!maze[u][v]){
					Sleep(TrackBar1->Position);
					maze[u][v] = 0;
					StringGrid2->Cells[v][u] = maze[u][v];
					step.dir = step.dir+=1;
					push(step);
					step.x = u; step.y = v; step.dir = 0;
					StringGrid2->Refresh();
					step.x = u; step.y = v; step.dir = 0 ;
			}
			else step.dir+=1;

			Sleep(TrackBar1->Position);
			maze[i][j] = 5;
			StringGrid2->Cells[j][i] = maze[i][j];
			StringGrid2->Refresh();
		}
	 }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::StringGrid3DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State)
{
		AnsiString text = StringGrid3->Cells[ACol][ARow];
		if (text == "0")
			StringGrid3->Canvas->Brush->Color = TColor RGB(198, 203, 239);
		else if(text == "1")
			StringGrid3->Canvas->Brush->Color = TColor RGB(129, 134, 213);
		else if(text == "2")
			StringGrid3->Canvas->Brush->Color = TColor RGB(73, 76, 162);
		else if(text == "3")
			StringGrid3->Canvas->Brush->Color = TColor RGB(255, 227, 227);
		else if(text == "4")
			StringGrid3->Canvas->Brush->Color = TColor RGB(255, 230, 188);
		else if(text == "5")
			StringGrid3->Canvas->Brush->Color = TColor RGB(229, 220, 195);
		StringGrid3->Canvas->FillRect(Rect);
}
//---------------------------------------------------------------------------



