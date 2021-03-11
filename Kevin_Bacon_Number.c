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

struct movie* createNodeMovie(char *, int);//Film düðümünü oluþturma fonksiyonu
struct artist* createNodeArtist(char *, int);//Oyuncu düðümünü oluþturma fonksiyonu
struct movieGraph* createMovieGraph(int);//Filmlerden oyunculara grafýný oluþturma fonksiyonu
struct artistGraph* createArtistGraph(int);//Oyunculardan filmlere grafýný oluþturma fonksiyonu
void addEdge(struct movieGraph*, struct artistGraph*, int, int, char*, char* );//Filmler ve oyuncular düðümleri arasýnda baðlantýlarýn kurulmasý fonksiyonu
QUEUE *createQueue(void);//Kuyruk oluþturma fonksiyonu
int dequeue(QUEUE*);//Kuyruktan düðüm çekme fonksiyonu
void enqueue(QUEUE*,int,char*);//Kuyruða düðüm ekleme fonksiyonu
int FindKevinBaconNumber(struct artistGraph*,struct movieGraph*,int,int,int*,char**);//Tüm oyuncular için Kevin Bacon sayýsýnýn Breadth First Search ile hesaplanmasý
void FindConnection(struct artistGraph*,struct movieGraph*,int, int,char **,struct artist**,struct movie**,char*);//Ýstenilen oyuncu için Kevin Bacon sayýsýný hesaplama ve Kevin Bacon ile baðlantýsýný bulan fonksiyonu
void edit(char*);//Oyuncu isimlerini düzgün þekilde yazdýrma fonksiyonu

int main(){
	
	int i = 0, k, j;
	int kontrol = 0;
	int sayac1 = 0;//Film indexlerinin tutulacaðý deðiþken
	int sayac2 = 0;//Oyuncu indexlerinin tutulacaðý deðiþken
	int movienumber = 1;//Film sayýsýnýn tutulduðu deðiþken
	int kbn;//Kevin Bacon sayýsýnýn tutulduðu deðiþken
	long long int artistnumber = 0;//Oyuncu sayýsýnýn tutulduðu deðiþken 
	int kevinbacon[100];//Her bir Kevin Bacon sayýsýndan ne kadar olduðunun tutulduðu dizi
	int max = 0;
	const char filename[20];//Dosya adýnýn tutlduðu dizi
	char temp[MAX] = {};//Dosyadan okunan film ve oyuncu isimlerinin tutulduðu dizi
	char temp1[MAX] = {};//Dosyadan okunan film adýnýn tutulduðu dizi
	char tmp;//Dosyadan karakter okumasý için kullanýlan deðiken
	char **artiststr;//Oyuncularýn indexlerine göre isimlerinin tutulduðu dizi
	char acname[MAX];//Kevin Bacon sayýsý aranan oyuncunun tutulduðu dizi
	char search[MAX] = "Bacon, Kevin";//Breadth First Number ile aranacak kelimenin tutulduðu dizi
	char s[MAX];
	FILE *fs;
	struct movieGraph *moviegraph;//Her filmin o filmde oynayan oyuncularla baðlantýsý olan graf
	struct artistGraph *artistgraph;//Her oyuncunun oynadýðý filmlerle baðlantýsý olan graf
	struct movie* mov = NULL;//Oyuncunun Kevin Bacon ile baðlantýsýnda bulunan filmlerin tutulduðu linkli liste
	struct artist* art = NULL;//Oyuncunun Kevin Bacon ile baðlantýsýnda bulunan oyuncunun tutulduðu linkli liste
	
	printf("Dosya adini giriniz(.txt uzantisi ile beraber) ");
	scanf( "%s", filename );
	gets(s);
	fs = fopen(filename, "r");
	while(!feof(fs)){
		tmp = getc(fs);
		if(tmp == '/') {
			
			artistnumber++;//Tahmini oyunucu sayýsýsýn hesaplanmasý
		}
		if(tmp == '\n'){
			
			movienumber++;//Tahmini film sayýsýnýn hesaplanmasý
		}
	}
	fclose(fs);

	artiststr = (char**)malloc(artistnumber*sizeof(char*));//Ýndisler üzerinden oyuncu isimlerine ulaþýlan dizinin oluþturulmasý
	
	for(i = 0; i < artistnumber; i++)
		artiststr[i] = (char*)malloc(500*sizeof(char));
		
	moviegraph = createMovieGraph(movienumber);//Film grafýnýn oluþturulmasý
	artistgraph = createArtistGraph(artistnumber);//Oyuncu grafýnýn oluþturulmasý
	i = 0;
	fs = fopen(filename, "r" );
	while(!feof(fs)){//Dosyadan oyuncu ve film bilgilerinin alýnmasý
		
		tmp = getc(fs);
		if ((tmp != '/') && (tmp != '\n')){//Her oyuncu '/' ile birbirinden ayrýlmýþtýr ayný þekilde yeni filme geçilirken '\n' karakteri yer alýr
			
			temp[i] = tmp;// '/' görene kadar karakter karakter okunarak film ya da oyuncu adý alýnýr
			i++;
		}
		else{
		
			kontrol++;//Dosyadan alýnan kelimenin film adý mý yoksa oyuncu adý mý olup olmadýðýnýn kontrolü
			if (kontrol > 1){//Eðer temp dizisinde oyuncu ismi saklanýyorsa
			
				k = 0;				
				while((k < sayac2) && (strcmp(temp,artiststr[k])))//Dosyadan alýnan oyuncu isminin daha önceden eklenip eklenmediðinin kontolü
					k++;
				
				if(k != sayac2)//Eðer Oyuncu daha önceden iþleme alýndýysa
					addEdge(moviegraph,artistgraph,sayac1-1,k,temp,temp1);//Oyuncu ilgili filmin komþu düðümü olarak ayarlanýr ve oyuncu grafýnda ilgili film komþu düðüm yapýlýr
				else{//Oyuncu ilk kez iþleme alýnýyorsa
				
					addEdge(moviegraph,artistgraph,sayac1-1,sayac2,temp,temp1);//Yeni gelen oyuncu ilgili filmin komþu düðümü olarak ayarlanýr ve oyuncu grafýnda ilgili film komþu düðüm yapýlýr
					strcpy(artiststr[sayac2], temp);//Oyuncu artiststr[] dizisine eklenir
					sayac2++;//Oyuncu sayýsýnýn tutulduðu sayac2 artýrýlýr
				}
		
				i = 0;
				j = 0;
				while(temp[j]){//temp[] dizisinin null yapýlmasý
					temp[j] = '\0';
					j++;
				}
			}
			else if ( kontrol == 1 ){//kontrol 1 ise dosyadan film adý alýnmýþ demektir
				
				strcpy(temp1,temp);//Film adýnýn kaybolmamasý için temp1 dizisinde saklanýr
				sayac1++;//Film sayýsýnýn tutulduðu sayac1 artýrýlýr
				i = 0;
				j = 0;
				while(temp[j]){//temp[] dizisinin null yapýlmasý
					temp[j] = '\0';
					j++;
				}			
			}
			
			if (tmp == '\n')//Her yeni filme geçildiðinde kontrol deðiþkeni 0 yapýlýr
				kontrol = 0;			
		}
	}
	fclose(fs);
	
	for (i = 0; i < 100; i++)//Her bir KeVin Bacon sayýsýnda kaç kiþi olduðunun tutulduðu dizinin sýfýrlanmasý
		kevinbacon[i] = 0;
	
	k = 0;
	while(k < sayac2){//her bir oyuncunun KEvin Bacon sayýsý hesaplanarak Kevin Bacon sayýlarýna göre oyuncu sayýsýnýn bulunmasý
		
		kevinbacon[FindKevinBaconNumber(artistgraph,moviegraph,k,sayac2,&max,artiststr)+1]++;
		k++;
	}

	for(i = 1; i < max + 2; i++ ){//Her bir KeVin Bacon sayýsýnda kaç kiþi olduðunun ekrana yazdýrýlmasý
		
		printf("Kevin Bacon sayisi %d olan oyuncu sayisi: %d\n", i-1, kevinbacon[i]);
	}
	printf("Kevin Bacon sayisi sonsuz olan oyuncu sayisi: %d\n", kevinbacon[0]);
	
	do{
		art = NULL;//Aranan oyuncunun Kevin Bacon ile olan baðlantýsýnda bulunanan oyuncularýn saklandýðý linkli liste
		mov = NULL;//Aranan oyuncunun Kevin Bacon ile olan baðlantýsýnda bulunanan filmlerin saklandýðý linkli liste
		strcpy(search,"Bacon, Kevin");
		printf("\nAktorun adini giriniz(Soyisim, Isim(Bacon, Kevin) seklinde giriniz.Bitirmek icin e tusuna basin): ");
		gets(acname);
	
		i = 0;
		while((strcmp(acname,artiststr[i])) && (i < sayac2))//Kevin Bacon sayýsý aranan oyuncunun indisinin hesaplanmasý
			i++;
	
		if (i != sayac2){
			
			kbn = FindKevinBaconNumber(artistgraph,moviegraph,i,sayac2,&max,artiststr);//Oyuncunun Kevin Bacon sayýsýnýn hesaplanmasý
			if (kbn != -1){//Eðer aranan oyuncunun Kevin Bacon sayýsý sonsuz deðilse 
			
				edit(acname);//Oyuncu ismi düzgün bir formatta düzenlenir
				printf("\n%s's Kevin Bacon sayisi: %d\n", acname, kbn);//Oyuncunun Kevin Bacon sayýsý ekrana yazdýrýlýr
				FindConnection(artistgraph,moviegraph,i,sayac2,artiststr,&art,&mov,search);//Oyuncunun Kevin Bacon ile olan baðlantýsýnýn bulunmasý
				edit(art->artistname);
			
				while(art->next != NULL){
	
					edit(art->next->artistname);
					printf("%s - %s: %s\n",art->artistname,art->next->artistname,mov->moviename);//Sýrasýyla aradaki baðlantýda bulunan oyuncular ve filmlerin yazdýrýlmasý
					art = art->next;
					mov = mov->next;
				}
			}
			else//Eðer dönüþ deðeri -1 ise aranan oyuncunun Kevin Bacon sayýsý sonsuzdur
				printf("Kevin Bacon sayisi sonsuz");
		}
		else{
			
			printf("Aradiginiz oyuncu listede yer almamaktadir...");
		}
			
	}while(strcmp(acname,"e"));//e'ye basýldýðýnda iþlem bitirilir

	return 0;
}

struct movie* createNodeMovie(char namee[], int numm){//Dosyadan okunan her film için düðüm oluþturulmasý
	
	struct movie* newmovie = malloc(sizeof(struct movie));
	newmovie->nummovie = numm;
	strcpy(newmovie->moviename,namee);
	newmovie->next = NULL;
	return newmovie;
}

struct artist* createNodeArtist(char name[], int num){//Dosyadan okunan her yeni oyuncu için düðüm oluþturulmasý
	
	struct artist* newartist = malloc(sizeof(struct artist));
	newartist->numartist = num;
	strcpy(newartist->artistname,name);
	newartist->next = NULL;
	return newartist;
}

struct movieGraph* createMovieGraph(int movienum){//Filmlerden, o filmde oynayan oyunculara baðlantý olucak þekilde grafýn oluþturulmasý
	
	int i;
	struct movieGraph* moviegraph = malloc(sizeof(struct movieGraph));
	moviegraph->adjLists = malloc(movienum*sizeof(struct artist*));
	for (i = 0; i < movienum; i++)
		moviegraph->adjLists[i] = NULL;
	return moviegraph;
}

struct artistGraph* createArtistGraph(int artistnum){//Oyunculardan oynadýklarý filmlere baðlantý olacak þekilde grafýn oluþturulmasý
	
	int i;
	struct artistGraph* artistgraph = malloc(sizeof(struct artistGraph));
	artistgraph->adjLists = malloc(artistnum*sizeof(struct movie*));

	for (i = 0; i < artistnum; i++)
		artistgraph->adjLists[i] = NULL;
		
	return artistgraph;
}

void addEdge(struct movieGraph* mg, struct artistGraph* ag, int src, int dest, char art[], char mov[]){//Yeni gelen oyuncunun grafa eklenmesi
	
	struct artist *newartist = createNodeArtist(art,dest);//Oyuncu için yeni bir düðüm oluþturulur
	newartist->next = mg->adjLists[src];//Oyuncu düðümü komþuluk listesine eklenir
	mg->adjLists[src] = newartist;
	struct movie *newmovie = createNodeMovie(mov,src);//Film için yeni bir düðüm oluþturulur
	newmovie->next = ag->adjLists[dest];//Film düðümü komþuluk listesine eklenir
	ag->adjLists[dest] = newmovie;
}

QUEUE *createQueue(){//Kuyruk oluþturma
	
	QUEUE *q;
	q = (QUEUE*)malloc(sizeof(QUEUE));//kuyruk için yer açýlmasý
	if(q == NULL){
		
		printf("Error\n");
		exit(0);
	}
	else{
		
		q->front = q->rear = NULL;//kuyruðun ön ve arka deðerleri null yapýlýr
		return q;
	}
}

void enqueue(QUEUE *q, int num, char name[]){//Kuyruða oyuncu eklenmesi
	
	struct artist* tmp = createNodeArtist(name,num);//Yeni bir oyuncu düðümünün oluþturulmasý
	
	if(q->front == NULL){//Eðer kuyruk boþsa

		q->front = q->rear = tmp;//Yeni gelen düðüm kuyruðun ön ve arka deðeri olarak ayarlanýr	
	}
	else{//Kuyruk boþ deðilse
	
		q->rear->next = tmp;//Yeni gelen oyuncu kuyruðun sonuna eklenir
		q->rear = tmp;
	}
}

int dequeue(QUEUE *q){//Kuruðun baþýndaki düðümün kuyruktan alýnmasý
	
	struct artist* tmp;
	int index;
	
	if(q->front == NULL)//Eðer kuruk boþsa
		return -1;
	
	index = q->front->numartist;//kuyruktan çekilecek oyuncunun indeksinin saklanmasý
	tmp = q->front;//Kuyruðun baþýndaki düðümün tmp düðümünde saklanmasý
	q->front = tmp->next;//Kuyruðun baþýna çýkarýlan düðümün bir sonraki düðümü getirilir
	free(tmp);
	
	if(q->front == NULL)//Eðer kuyrukta eleman kalmadýysa
		q->rear = NULL;
	
	return index;
}

int FindKevinBaconNumber(struct artistGraph* ag, struct movieGraph* mg, int artindex, int numofart, int* sayi, char **artiststr){//Breadth First Search ile aranan oyuncunun Kevin Bacon sayýsýnýn hesaplanmasý

	int visited[numofart], path[numofart], i, deqindex;
	QUEUE* q = createQueue();
	
	for (i = 0; i < numofart; i++){//visited[] ve path[] dizilerin baþlangýç deðeri olarak sýfýrlanmasý
		
		visited[i] = 0;//Gezilen düðüme bir daha gitmmemek için kullanýlan dizi
		path[i] = 0;//Aranan oyuncuya olan uzaklýðýn tutulduðu dizi
	}

	if(!strcmp(artiststr[artindex],"Bacon, Kevin"))//Eðer aranan oyuncu Kevin Bacon ise Kevin Bacon sayýsý 0 olduðu için 0 döndürülür
		return 0;
			
	enqueue(q,artindex,artiststr[artindex]);//Ýlk olarak aranan oyunucunun kuyruða eklenmesi
	visited[artindex] = 1;//Yeniden ayný oyuncu düðümüne gitmmemek için visited[] dizisinde bu oyunucunun indexinin olduðu indis 1 yapýlýr
	deqindex = dequeue(q);//Oyuncu kuyruktan çýkarýlýr

	while(deqindex != -1){//Kuyruk boþ olmadýðý sürece arama devam eder
		
		struct movie* mov = ag->adjLists[deqindex];//Kevin Bacon sayýsý aranan oyuncunun oynadýðý film için film düðümünün oluþturulmasý
		while(mov != NULL){//Oyuncunun oynadýðý tüm filmler için 
				
			struct artist* art = mg->adjLists[mov->nummovie];//Bu filmlerde oynayan diðer oyuncular için oyuncu düðümünün oluþturulmasý
			while(art != NULL){//Bu filmde oynayan tüm oyuncular için
				
				if(visited[art->numartist] == 0){//Bu filmlerde oynayan oyuncu düðümüne daha önce gidilmediyse
		
				enqueue(q,art->numartist,art->artistname);//Komþu düðümdeki oyuncuyu kuyruða ekleme
				visited[art->numartist] = 1;//Bu oyuncu ile yeniden karþýlaþmamak için visited[] dizisinde deðeri 1 yapýlar
				path[art->numartist] = path[deqindex] + 1;//
				}
				if(!strcmp(art->artistname,"Bacon, Kevin")){//Eðer kontrol edilen oyuncu Kevin Bacon ise 
				
					if(*sayi < path[art->numartist])
						*sayi = path[art->numartist];//En büyük Kevin Bacon sayýsýnýn sayi deðiþkenþnde tutulmasý
				
					return path[art->numartist];//
				}
				art = art->next;//Komþu düðümdeki bir sonraki oyuncuya geçilmesi 
			}
			
			mov = mov->next;//Oyuncunun oynadýðý bir sonraki düðüme geçilmesi
		}
		deqindex = dequeue(q);//Kuyruktan oyuncunun çekilmesi
	}

	return -1;//Eðer Kevin Bacon'a ulaþýlmadýysa -1 döndür
}

void FindConnection(struct artistGraph* ag, struct movieGraph* mg, int artindex, int numofart, char **artiststr, struct artist** a, struct movie** m, char search[]){
	
	int visited[numofart], i, deqindex;
	QUEUE* q = createQueue();
	
	for (i = 0; i < numofart; i++)//Aranan oyuncuya olan uzaklýðýn tutulduðu dizinin sýfýrlanmasý
		visited[i] = 0;
	
	if(!strcmp(artiststr[artindex],search)){//Aranan oyuncu Kevin Bacon ise
		
		struct artist* art = createNodeArtist(artiststr[artindex],artindex);
		art->next = *a;
		*a = art;
		return;	
	}
	
	enqueue(q,artindex,artiststr[artindex]);//Ýlk olarak aranan oyunucunun kuyruða eklenmesi
	visited[artindex] = 1;//Yeniden ayný oyuncu düðümüne gitmmemek için visited[] dizisinde bu oyunucunun indexinin olduðu indis 1 yapýlýr
	deqindex = dequeue(q);//Oyuncu kuyruktan çýkarýlýr
	
	while(deqindex != -1){//Kuyruk boþ olmadýðý sürece arama devam eder
		
		struct movie* mov = ag->adjLists[deqindex];//Kevin Bacon sayýsý aranan oyuncunun oynadýðý film için film düðümünün oluþturulmasý
		while(mov != NULL){//Oyuncunun oynadýðý tüm filmler için 
			
			struct artist* art = mg->adjLists[mov->nummovie];//Bu filmlerde oynayan diðer oyuncular için oyuncu düðümünün oluþturulmasý
			while(art != NULL){//Bu filmde oynayan tüm oyuncular için
				
				if(visited[art->numartist] == 0){//Bu filmlerde oynayan oyuncu düðümüne daha önce gidilmediyse
					
					enqueue(q,art->numartist,art->artistname);//Komþu düðümdeki oyuncuyu kuyruða ekleme
					visited[art->numartist] = 1;//Bu oyuncu ile yeniden karþýlaþmamak için visited[] dizisinde deðeri 1 yapýlar
				}
				if(!strcmp(art->artistname,search)){//Eðer aranan oyuncuya ulaþýldýysa
					
					struct artist* artt = createNodeArtist(search,art->numartist);
					artt->next = *a;//Baðlantýda bulunan oyuncunun linkli listeye eklenmesi
					*a = artt;
					struct movie* movv = createNodeMovie(mov->moviename,mov->nummovie);
					movv->next = *m;//Baðlantýda bulunan filmin linkli listeye eklenmesi
					*m = movv;
					strcpy(search,artiststr[deqindex]);//Kevin Bacon'dan önceki oyuncunun aranacak kelime olarak ayarlanmasý
					FindConnection(ag,mg,artindex,numofart,artiststr,a,m,search);//Recursive olarak Kevin Bacon sayýsý aranan oyuncuya ulaþana kadar aramanýn devam eder. Böylece aradaki baðlantý bulunmuþ olur.
					return;
				}
				
				art = art->next;//Komþu düðümdeki bir sonraki oyuncuya geçilmesi 
			}
			
			mov = mov->next;//Oyuncunun oynadýðý bir sonraki düðüme geçilmesi
		}
		deqindex = dequeue(q);//Kuyruktan oyuncunun çekilmesi
	}
}

void edit(char str[]){//Oyuncu isminin düzgün formata çevrilmesi
	
	char tmp[MAX] = {}, tmp1[MAX] = {};
	int i = 0, j = 0;

	while(str[i] != ','){//Öncelikle oyuncunun soyisimi tmp[] dizisine atanýr
		
		tmp[i+1] = str[i];
		i++;
	}
	tmp[0] = ' ';
	
	i += 2;
	while(str[i] != '\0'){//Oyuncunun adýnýn tmp1[] dizisine atanýr

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
	strcpy(str,tmp1);//Düzgün þekilde yazýlmýþ olan oyuncu ismi str[] dizisine kopyalanýr
}

