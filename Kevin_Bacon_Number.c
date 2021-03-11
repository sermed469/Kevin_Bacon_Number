#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 500

struct movie{
	
	int nummovie;
	char moviename[MAX];
	struct movie *next;
};

struct artist{
	
	int numartist;
	char artistname[MAX];
	struct artist *next;
};

struct movieGraph{//Filmlerden oyunculara graf
	
	int numofmovies;
	struct artist** adjLists;
};

struct artistGraph{//Oyunculardan filmlere graf
	
	int numofartists;
	struct movie** adjLists;
};

typedef struct queue{
	
	struct artist *front, *rear;
	
}QUEUE;

struct movie* createNodeMovie(char *, int);//Film d���m�n� olu�turma fonksiyonu
struct artist* createNodeArtist(char *, int);//Oyuncu d���m�n� olu�turma fonksiyonu
struct movieGraph* createMovieGraph(int);//Filmlerden oyunculara graf�n� olu�turma fonksiyonu
struct artistGraph* createArtistGraph(int);//Oyunculardan filmlere graf�n� olu�turma fonksiyonu
void addEdge(struct movieGraph*, struct artistGraph*, int, int, char*, char* );//Filmler ve oyuncular d���mleri aras�nda ba�lant�lar�n kurulmas� fonksiyonu
QUEUE *createQueue(void);//Kuyruk olu�turma fonksiyonu
int dequeue(QUEUE*);//Kuyruktan d���m �ekme fonksiyonu
void enqueue(QUEUE*,int,char*);//Kuyru�a d���m ekleme fonksiyonu
int FindKevinBaconNumber(struct artistGraph*,struct movieGraph*,int,int,int*,char**);//T�m oyuncular i�in Kevin Bacon say�s�n�n Breadth First Search ile hesaplanmas�
void FindConnection(struct artistGraph*,struct movieGraph*,int, int,char **,struct artist**,struct movie**,char*);//�stenilen oyuncu i�in Kevin Bacon say�s�n� hesaplama ve Kevin Bacon ile ba�lant�s�n� bulan fonksiyonu
void edit(char*);//Oyuncu isimlerini d�zg�n �ekilde yazd�rma fonksiyonu

int main(){
	
	int i = 0, k, j;
	int kontrol = 0;
	int sayac1 = 0;//Film indexlerinin tutulaca�� de�i�ken
	int sayac2 = 0;//Oyuncu indexlerinin tutulaca�� de�i�ken
	int movienumber = 1;//Film say�s�n�n tutuldu�u de�i�ken
	int kbn;//Kevin Bacon say�s�n�n tutuldu�u de�i�ken
	long long int artistnumber = 0;//Oyuncu say�s�n�n tutuldu�u de�i�ken 
	int kevinbacon[100];//Her bir Kevin Bacon say�s�ndan ne kadar oldu�unun tutuldu�u dizi
	int max = 0;
	const char filename[20];//Dosya ad�n�n tutldu�u dizi
	char temp[MAX] = {};//Dosyadan okunan film ve oyuncu isimlerinin tutuldu�u dizi
	char temp1[MAX] = {};//Dosyadan okunan film ad�n�n tutuldu�u dizi
	char tmp;//Dosyadan karakter okumas� i�in kullan�lan de�iken
	char **artiststr;//Oyuncular�n indexlerine g�re isimlerinin tutuldu�u dizi
	char acname[MAX];//Kevin Bacon say�s� aranan oyuncunun tutuldu�u dizi
	char search[MAX] = "Bacon, Kevin";//Breadth First Number ile aranacak kelimenin tutuldu�u dizi
	char s[MAX];
	FILE *fs;
	struct movieGraph *moviegraph;//Her filmin o filmde oynayan oyuncularla ba�lant�s� olan graf
	struct artistGraph *artistgraph;//Her oyuncunun oynad��� filmlerle ba�lant�s� olan graf
	struct movie* mov = NULL;//Oyuncunun Kevin Bacon ile ba�lant�s�nda bulunan filmlerin tutuldu�u linkli liste
	struct artist* art = NULL;//Oyuncunun Kevin Bacon ile ba�lant�s�nda bulunan oyuncunun tutuldu�u linkli liste
	
	printf("Dosya adini giriniz(.txt uzantisi ile beraber) ");
	scanf( "%s", filename );
	gets(s);
	fs = fopen(filename, "r");
	while(!feof(fs)){
		tmp = getc(fs);
		if(tmp == '/') {
			
			artistnumber++;//Tahmini oyunucu say�s�s�n hesaplanmas�
		}
		if(tmp == '\n'){
			
			movienumber++;//Tahmini film say�s�n�n hesaplanmas�
		}
	}
	fclose(fs);

	artiststr = (char**)malloc(artistnumber*sizeof(char*));//�ndisler �zerinden oyuncu isimlerine ula��lan dizinin olu�turulmas�
	
	for(i = 0; i < artistnumber; i++)
		artiststr[i] = (char*)malloc(500*sizeof(char));
		
	moviegraph = createMovieGraph(movienumber);//Film graf�n�n olu�turulmas�
	artistgraph = createArtistGraph(artistnumber);//Oyuncu graf�n�n olu�turulmas�
	i = 0;
	fs = fopen(filename, "r" );
	while(!feof(fs)){//Dosyadan oyuncu ve film bilgilerinin al�nmas�
		
		tmp = getc(fs);
		if ((tmp != '/') && (tmp != '\n')){//Her oyuncu '/' ile birbirinden ayr�lm��t�r ayn� �ekilde yeni filme ge�ilirken '\n' karakteri yer al�r
			
			temp[i] = tmp;// '/' g�rene kadar karakter karakter okunarak film ya da oyuncu ad� al�n�r
			i++;
		}
		else{
		
			kontrol++;//Dosyadan al�nan kelimenin film ad� m� yoksa oyuncu ad� m� olup olmad���n�n kontrol�
			if (kontrol > 1){//E�er temp dizisinde oyuncu ismi saklan�yorsa
			
				k = 0;				
				while((k < sayac2) && (strcmp(temp,artiststr[k])))//Dosyadan al�nan oyuncu isminin daha �nceden eklenip eklenmedi�inin kontol�
					k++;
				
				if(k != sayac2)//E�er Oyuncu daha �nceden i�leme al�nd�ysa
					addEdge(moviegraph,artistgraph,sayac1-1,k,temp,temp1);//Oyuncu ilgili filmin kom�u d���m� olarak ayarlan�r ve oyuncu graf�nda ilgili film kom�u d���m yap�l�r
				else{//Oyuncu ilk kez i�leme al�n�yorsa
				
					addEdge(moviegraph,artistgraph,sayac1-1,sayac2,temp,temp1);//Yeni gelen oyuncu ilgili filmin kom�u d���m� olarak ayarlan�r ve oyuncu graf�nda ilgili film kom�u d���m yap�l�r
					strcpy(artiststr[sayac2], temp);//Oyuncu artiststr[] dizisine eklenir
					sayac2++;//Oyuncu say�s�n�n tutuldu�u sayac2 art�r�l�r
				}
		
				i = 0;
				j = 0;
				while(temp[j]){//temp[] dizisinin null yap�lmas�
					temp[j] = '\0';
					j++;
				}
			}
			else if ( kontrol == 1 ){//kontrol 1 ise dosyadan film ad� al�nm�� demektir
				
				strcpy(temp1,temp);//Film ad�n�n kaybolmamas� i�in temp1 dizisinde saklan�r
				sayac1++;//Film say�s�n�n tutuldu�u sayac1 art�r�l�r
				i = 0;
				j = 0;
				while(temp[j]){//temp[] dizisinin null yap�lmas�
					temp[j] = '\0';
					j++;
				}			
			}
			
			if (tmp == '\n')//Her yeni filme ge�ildi�inde kontrol de�i�keni 0 yap�l�r
				kontrol = 0;			
		}
	}
	fclose(fs);
	
	for (i = 0; i < 100; i++)//Her bir KeVin Bacon say�s�nda ka� ki�i oldu�unun tutuldu�u dizinin s�f�rlanmas�
		kevinbacon[i] = 0;
	
	k = 0;
	while(k < sayac2){//her bir oyuncunun KEvin Bacon say�s� hesaplanarak Kevin Bacon say�lar�na g�re oyuncu say�s�n�n bulunmas�
		
		kevinbacon[FindKevinBaconNumber(artistgraph,moviegraph,k,sayac2,&max,artiststr)+1]++;
		k++;
	}

	for(i = 1; i < max + 2; i++ ){//Her bir KeVin Bacon say�s�nda ka� ki�i oldu�unun ekrana yazd�r�lmas�
		
		printf("Kevin Bacon sayisi %d olan oyuncu sayisi: %d\n", i-1, kevinbacon[i]);
	}
	printf("Kevin Bacon sayisi sonsuz olan oyuncu sayisi: %d\n", kevinbacon[0]);
	
	do{
		art = NULL;//Aranan oyuncunun Kevin Bacon ile olan ba�lant�s�nda bulunanan oyuncular�n sakland��� linkli liste
		mov = NULL;//Aranan oyuncunun Kevin Bacon ile olan ba�lant�s�nda bulunanan filmlerin sakland��� linkli liste
		strcpy(search,"Bacon, Kevin");
		printf("\nAktorun adini giriniz(Soyisim, Isim(Bacon, Kevin) seklinde giriniz.Bitirmek icin e tusuna basin): ");
		gets(acname);
	
		i = 0;
		while((strcmp(acname,artiststr[i])) && (i < sayac2))//Kevin Bacon say�s� aranan oyuncunun indisinin hesaplanmas�
			i++;
	
		if (i != sayac2){
			
			kbn = FindKevinBaconNumber(artistgraph,moviegraph,i,sayac2,&max,artiststr);//Oyuncunun Kevin Bacon say�s�n�n hesaplanmas�
			if (kbn != -1){//E�er aranan oyuncunun Kevin Bacon say�s� sonsuz de�ilse 
			
				edit(acname);//Oyuncu ismi d�zg�n bir formatta d�zenlenir
				printf("\n%s's Kevin Bacon sayisi: %d\n", acname, kbn);//Oyuncunun Kevin Bacon say�s� ekrana yazd�r�l�r
				FindConnection(artistgraph,moviegraph,i,sayac2,artiststr,&art,&mov,search);//Oyuncunun Kevin Bacon ile olan ba�lant�s�n�n bulunmas�
				edit(art->artistname);
			
				while(art->next != NULL){
	
					edit(art->next->artistname);
					printf("%s - %s: %s\n",art->artistname,art->next->artistname,mov->moviename);//S�ras�yla aradaki ba�lant�da bulunan oyuncular ve filmlerin yazd�r�lmas�
					art = art->next;
					mov = mov->next;
				}
			}
			else//E�er d�n�� de�eri -1 ise aranan oyuncunun Kevin Bacon say�s� sonsuzdur
				printf("Kevin Bacon sayisi sonsuz");
		}
		else{
			
			printf("Aradiginiz oyuncu listede yer almamaktadir...");
		}
			
	}while(strcmp(acname,"e"));//e'ye bas�ld���nda i�lem bitirilir

	return 0;
}

struct movie* createNodeMovie(char namee[], int numm){//Dosyadan okunan her film i�in d���m olu�turulmas�
	
	struct movie* newmovie = malloc(sizeof(struct movie));
	newmovie->nummovie = numm;
	strcpy(newmovie->moviename,namee);
	newmovie->next = NULL;
	return newmovie;
}

struct artist* createNodeArtist(char name[], int num){//Dosyadan okunan her yeni oyuncu i�in d���m olu�turulmas�
	
	struct artist* newartist = malloc(sizeof(struct artist));
	newartist->numartist = num;
	strcpy(newartist->artistname,name);
	newartist->next = NULL;
	return newartist;
}

struct movieGraph* createMovieGraph(int movienum){//Filmlerden, o filmde oynayan oyunculara ba�lant� olucak �ekilde graf�n olu�turulmas�
	
	int i;
	struct movieGraph* moviegraph = malloc(sizeof(struct movieGraph));
	moviegraph->adjLists = malloc(movienum*sizeof(struct artist*));
	for (i = 0; i < movienum; i++)
		moviegraph->adjLists[i] = NULL;
	return moviegraph;
}

struct artistGraph* createArtistGraph(int artistnum){//Oyunculardan oynad�klar� filmlere ba�lant� olacak �ekilde graf�n olu�turulmas�
	
	int i;
	struct artistGraph* artistgraph = malloc(sizeof(struct artistGraph));
	artistgraph->adjLists = malloc(artistnum*sizeof(struct movie*));

	for (i = 0; i < artistnum; i++)
		artistgraph->adjLists[i] = NULL;
		
	return artistgraph;
}

void addEdge(struct movieGraph* mg, struct artistGraph* ag, int src, int dest, char art[], char mov[]){//Yeni gelen oyuncunun grafa eklenmesi
	
	struct artist *newartist = createNodeArtist(art,dest);//Oyuncu i�in yeni bir d���m olu�turulur
	newartist->next = mg->adjLists[src];//Oyuncu d���m� kom�uluk listesine eklenir
	mg->adjLists[src] = newartist;
	struct movie *newmovie = createNodeMovie(mov,src);//Film i�in yeni bir d���m olu�turulur
	newmovie->next = ag->adjLists[dest];//Film d���m� kom�uluk listesine eklenir
	ag->adjLists[dest] = newmovie;
}

QUEUE *createQueue(){//Kuyruk olu�turma
	
	QUEUE *q;
	q = (QUEUE*)malloc(sizeof(QUEUE));//kuyruk i�in yer a��lmas�
	if(q == NULL){
		
		printf("Error\n");
		exit(0);
	}
	else{
		
		q->front = q->rear = NULL;//kuyru�un �n ve arka de�erleri null yap�l�r
		return q;
	}
}

void enqueue(QUEUE *q, int num, char name[]){//Kuyru�a oyuncu eklenmesi
	
	struct artist* tmp = createNodeArtist(name,num);//Yeni bir oyuncu d���m�n�n olu�turulmas�
	
	if(q->front == NULL){//E�er kuyruk bo�sa

		q->front = q->rear = tmp;//Yeni gelen d���m kuyru�un �n ve arka de�eri olarak ayarlan�r	
	}
	else{//Kuyruk bo� de�ilse
	
		q->rear->next = tmp;//Yeni gelen oyuncu kuyru�un sonuna eklenir
		q->rear = tmp;
	}
}

int dequeue(QUEUE *q){//Kuru�un ba��ndaki d���m�n kuyruktan al�nmas�
	
	struct artist* tmp;
	int index;
	
	if(q->front == NULL)//E�er kuruk bo�sa
		return -1;
	
	index = q->front->numartist;//kuyruktan �ekilecek oyuncunun indeksinin saklanmas�
	tmp = q->front;//Kuyru�un ba��ndaki d���m�n tmp d���m�nde saklanmas�
	q->front = tmp->next;//Kuyru�un ba��na ��kar�lan d���m�n bir sonraki d���m� getirilir
	free(tmp);
	
	if(q->front == NULL)//E�er kuyrukta eleman kalmad�ysa
		q->rear = NULL;
	
	return index;
}

int FindKevinBaconNumber(struct artistGraph* ag, struct movieGraph* mg, int artindex, int numofart, int* sayi, char **artiststr){//Breadth First Search ile aranan oyuncunun Kevin Bacon say�s�n�n hesaplanmas�

	int visited[numofart], path[numofart], i, deqindex;
	QUEUE* q = createQueue();
	
	for (i = 0; i < numofart; i++){//visited[] ve path[] dizilerin ba�lang�� de�eri olarak s�f�rlanmas�
		
		visited[i] = 0;//Gezilen d���me bir daha gitmmemek i�in kullan�lan dizi
		path[i] = 0;//Aranan oyuncuya olan uzakl���n tutuldu�u dizi
	}

	if(!strcmp(artiststr[artindex],"Bacon, Kevin"))//E�er aranan oyuncu Kevin Bacon ise Kevin Bacon say�s� 0 oldu�u i�in 0 d�nd�r�l�r
		return 0;
			
	enqueue(q,artindex,artiststr[artindex]);//�lk olarak aranan oyunucunun kuyru�a eklenmesi
	visited[artindex] = 1;//Yeniden ayn� oyuncu d���m�ne gitmmemek i�in visited[] dizisinde bu oyunucunun indexinin oldu�u indis 1 yap�l�r
	deqindex = dequeue(q);//Oyuncu kuyruktan ��kar�l�r

	while(deqindex != -1){//Kuyruk bo� olmad��� s�rece arama devam eder
		
		struct movie* mov = ag->adjLists[deqindex];//Kevin Bacon say�s� aranan oyuncunun oynad��� film i�in film d���m�n�n olu�turulmas�
		while(mov != NULL){//Oyuncunun oynad��� t�m filmler i�in 
				
			struct artist* art = mg->adjLists[mov->nummovie];//Bu filmlerde oynayan di�er oyuncular i�in oyuncu d���m�n�n olu�turulmas�
			while(art != NULL){//Bu filmde oynayan t�m oyuncular i�in
				
				if(visited[art->numartist] == 0){//Bu filmlerde oynayan oyuncu d���m�ne daha �nce gidilmediyse
		
				enqueue(q,art->numartist,art->artistname);//Kom�u d���mdeki oyuncuyu kuyru�a ekleme
				visited[art->numartist] = 1;//Bu oyuncu ile yeniden kar��la�mamak i�in visited[] dizisinde de�eri 1 yap�lar
				path[art->numartist] = path[deqindex] + 1;//
				}
				if(!strcmp(art->artistname,"Bacon, Kevin")){//E�er kontrol edilen oyuncu Kevin Bacon ise 
				
					if(*sayi < path[art->numartist])
						*sayi = path[art->numartist];//En b�y�k Kevin Bacon say�s�n�n sayi de�i�ken�nde tutulmas�
				
					return path[art->numartist];//
				}
				art = art->next;//Kom�u d���mdeki bir sonraki oyuncuya ge�ilmesi 
			}
			
			mov = mov->next;//Oyuncunun oynad��� bir sonraki d���me ge�ilmesi
		}
		deqindex = dequeue(q);//Kuyruktan oyuncunun �ekilmesi
	}

	return -1;//E�er Kevin Bacon'a ula��lmad�ysa -1 d�nd�r
}

void FindConnection(struct artistGraph* ag, struct movieGraph* mg, int artindex, int numofart, char **artiststr, struct artist** a, struct movie** m, char search[]){
	
	int visited[numofart], i, deqindex;
	QUEUE* q = createQueue();
	
	for (i = 0; i < numofart; i++)//Aranan oyuncuya olan uzakl���n tutuldu�u dizinin s�f�rlanmas�
		visited[i] = 0;
	
	if(!strcmp(artiststr[artindex],search)){//Aranan oyuncu Kevin Bacon ise
		
		struct artist* art = createNodeArtist(artiststr[artindex],artindex);
		art->next = *a;
		*a = art;
		return;	
	}
	
	enqueue(q,artindex,artiststr[artindex]);//�lk olarak aranan oyunucunun kuyru�a eklenmesi
	visited[artindex] = 1;//Yeniden ayn� oyuncu d���m�ne gitmmemek i�in visited[] dizisinde bu oyunucunun indexinin oldu�u indis 1 yap�l�r
	deqindex = dequeue(q);//Oyuncu kuyruktan ��kar�l�r
	
	while(deqindex != -1){//Kuyruk bo� olmad��� s�rece arama devam eder
		
		struct movie* mov = ag->adjLists[deqindex];//Kevin Bacon say�s� aranan oyuncunun oynad��� film i�in film d���m�n�n olu�turulmas�
		while(mov != NULL){//Oyuncunun oynad��� t�m filmler i�in 
			
			struct artist* art = mg->adjLists[mov->nummovie];//Bu filmlerde oynayan di�er oyuncular i�in oyuncu d���m�n�n olu�turulmas�
			while(art != NULL){//Bu filmde oynayan t�m oyuncular i�in
				
				if(visited[art->numartist] == 0){//Bu filmlerde oynayan oyuncu d���m�ne daha �nce gidilmediyse
					
					enqueue(q,art->numartist,art->artistname);//Kom�u d���mdeki oyuncuyu kuyru�a ekleme
					visited[art->numartist] = 1;//Bu oyuncu ile yeniden kar��la�mamak i�in visited[] dizisinde de�eri 1 yap�lar
				}
				if(!strcmp(art->artistname,search)){//E�er aranan oyuncuya ula��ld�ysa
					
					struct artist* artt = createNodeArtist(search,art->numartist);
					artt->next = *a;//Ba�lant�da bulunan oyuncunun linkli listeye eklenmesi
					*a = artt;
					struct movie* movv = createNodeMovie(mov->moviename,mov->nummovie);
					movv->next = *m;//Ba�lant�da bulunan filmin linkli listeye eklenmesi
					*m = movv;
					strcpy(search,artiststr[deqindex]);//Kevin Bacon'dan �nceki oyuncunun aranacak kelime olarak ayarlanmas�
					FindConnection(ag,mg,artindex,numofart,artiststr,a,m,search);//Recursive olarak Kevin Bacon say�s� aranan oyuncuya ula�ana kadar araman�n devam eder. B�ylece aradaki ba�lant� bulunmu� olur.
					return;
				}
				
				art = art->next;//Kom�u d���mdeki bir sonraki oyuncuya ge�ilmesi 
			}
			
			mov = mov->next;//Oyuncunun oynad��� bir sonraki d���me ge�ilmesi
		}
		deqindex = dequeue(q);//Kuyruktan oyuncunun �ekilmesi
	}
}

void edit(char str[]){//Oyuncu isminin d�zg�n formata �evrilmesi
	
	char tmp[MAX] = {}, tmp1[MAX] = {};
	int i = 0, j = 0;

	while(str[i] != ','){//�ncelikle oyuncunun soyisimi tmp[] dizisine atan�r
		
		tmp[i+1] = str[i];
		i++;
	}
	tmp[0] = ' ';
	
	i += 2;
	while(str[i] != '\0'){//Oyuncunun ad�n�n tmp1[] dizisine atan�r

		tmp1[j] = str[i];
		i++;
		j++;
	}
	
	i = 0;
	while(tmp[i] != '\0'){//tmp1[] dizisine oyuncunun soyismi eklenir

		tmp1[j] = tmp[i];
		i++;
		j++;
	}
	strcpy(str,tmp1);//D�zg�n �ekilde yaz�lm�� olan oyuncu ismi str[] dizisine kopyalan�r
}

