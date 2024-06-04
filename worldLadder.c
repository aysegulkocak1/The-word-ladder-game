#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100
# define MAX_WORD_NUM 1000

typedef struct Stack {
	char item[MAX_WORD_NUM][MAX];
	int top;
}Stack;

typedef struct QueueList {
    Stack *stack;          
    struct QueueList *next; 
} QueueList;

typedef struct {
	QueueList *front; // kuyrugun basi
    QueueList *rear; // kuyrugun sonu
}Queue;

typedef struct Node {
  char *word;
  struct Node *next;
} Node;

Stack* createStack();
int isFull(Stack *s);
int isEmpty(Stack *s);
void push(Stack *stack, char *word);
char* pop(Stack *stack);
Stack *copyStack(Stack *stack);

Queue* createQueue();
int isQueueEmpty(Queue *queue);
void enqueue(Queue *queue, Stack *stack);
Stack* dequeue(Queue *queue);


int isWordSuitable(char *startWord, char *currentWord);
int isInUsedWords(Node *head, char *word);
void readAndPushWordsToQueue(char *filename, char *startWord, Queue *queue, Stack *current, Node **usedWordsHead);
int findPath(char *filename,char* startWord,char*stopWord,Queue * queue,Node **usedWordsHead);

int main(){
	char *startWord = (char*)malloc(MAX*sizeof(char));
    char *stopWord = (char*)malloc(MAX*sizeof(char));
	printf("Baslangic kelimesini giriniz:\n");
	scanf("%s",startWord);
	printf("Hedef kelimeyi giriniz:\n");
	scanf("%s",stopWord);
	
	
	 int i;
	
	Queue *q =createQueue();
	printf("queue is created\n");
	Node *usedWordsHead = NULL;
	findPath("dictionary.txt",startWord,stopWord,q,&usedWordsHead);
	
	
	return 0;
}

//@brief  Stack olusturur.
// @param first Bellek ayirmak icin malloc fonksiyonu kullanilir.
// @param second Stack'in top degeri -1 olarak ayarlanir.
// @return Olusturulan Stack'in isaretcisini dondurur.
Stack* createStack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->top = -1;
    return stack;
}

/**
 * @brief Stack'in dolu olup olmadigini kontrol eder.
 * 
 * @param first Stack'in isaretcisi alýnýr. stack' in topu MAX_WORD_NUM dan kücükse bostur
 * @return Stack dolu ise 1, degilse 0 döndürülür.
 */
int isFull(Stack *s){
	if(s->top == MAX_WORD_NUM){
		return 1;
	}
	else return 0;
}

/**
 * @brief Stack'in bos olup olmadigini kontrol eder.
 * 
 * @param first Stack'in isaretcisi alýnýr. stack' in topu -1 ise  bostur
 * @return Stack bos ise 1, degilse 0 döndürülür.
 */
int isEmpty(Stack *s){
	if (s->top ==-1){
		return 1;
	}
	else return 0;
}

/**
* @brief Stack e kelime ekler
*@param first Eðer Stack dolu ise, kelime eklenemez ve hata mesaji yazdirilir
*@param second  Stack'in top degeri bir arttirilir
*@param third  Kelime stack'in belirtilen indeksine kopyalanir 
*/
void push(Stack *stack, char *data) {
    if (isFull(stack)) {
        printf("Stack dolu, push yapamazsýnýz!\n");
        return;
    }
    stack->top++;
    strcpy(stack->item[stack->top], data);
}

/**
* @brief Stack in en üstteki elemanini stackten cikarir.
*@param first Eðer Stack bos ise, kelime cikarilmaz ve hata mesaji yazdirilir
*@param second  Stack'in en ustunde bulunan kelime tutulur  ve top degeri   bir azaltilir
*@return Tutulan kelime donderilir.
*/

char* pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack boþ, pop yapamazsýnýz!\n");
        return NULL;
    }
    char *data = stack->item[stack->top];
    stack->top--;
    return data;
}

/**
*@ brief Parametre olarak verilen stack kopyalanir.
*@ param first Yeni stack olusturulur.
*@param second For dongüsü icinde stackteki elemanlar yeni stacke push edilir
@return Yeni stackin isaretcisi donderilir.
*/
Stack *copyStack(Stack *stack) {
    Stack *newStack = createStack();
    int i;
    for (i = 0; i <= stack->top; i++) {
        push(newStack, stack->item[i]);
    }

    return newStack;
}


/**
@brief Kuyruk olusturur.
@param First Kuyruk icin bellekte yer ayirilir.
@param Second Kuyrugun basi ve sonu NULL olarak atanir.
@return Kuyrugun isaretcisini dondurur.
*/
Queue* createQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

/**
@brief Kuyrugun bos olup olmadigini kontrol eder
@param Kuyrugun basi NULL ise kuyruk bostur.
@return Kuyruk bos ise 1 degilse 0 donderir.
*/
int isQueueEmpty(Queue *queue) {
    return queue->front == NULL;
}

/**
@brief Kuyruga stack ekler
@param First Yeni bir QueueList dugumu oluþturulur
@param Second Dugumun stack alanina, verilen Stack'in isaretcisi atanir
@param Third Dugumun nexti NULL olarak ayarlanir
@param Fourth Eger kuyruk bos ise kuyrugun front ve rear'i bu yeni dugume isaret eder
@param Fifth Kuyruk dolu ise, yeni dugum rear dugumunun next'ine eklenir ve rear guncellenir
*/
void enqueue(Queue *queue, Stack *stack) {
    QueueList *node = (QueueList*)malloc(sizeof(QueueList));
    node->stack = stack;
    node->next = NULL;

    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
}


/**
*@ brief Kuyruktan stack cikarir
*@param First Eger kuyruk bos ise islem yapilamaz, kullaniciya uyari verilir ve NULL dondurulur
*@param Second Kuyrugun basindaki QueueList dugumu gecici bir degiskene atanir
*@param Third Kuyrugun front'u bir sonraki dugume kaydirilir
*@param Fourth Eger kuyruk bos ise kuyrugun rear'i da NULL olarak guncellenir
*@param Fifth Kuyrugun basindaki Stack'in isaretcisi gecici degiskene atanir ve gecici dugum bellekten serbest birakilir
*@return Kuyrugun basindaki Stack'in isaretcisi dondurulur
*/
Stack* dequeue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        printf("Kuyruk bos!\n");
        return NULL;
    }
    QueueList *temp = queue->front; 
    queue->front = queue->front->next; // FIFO
    if (isQueueEmpty(queue)) {
        queue->rear = NULL;
    }
    Stack *stack = temp->stack;
    free(temp);
    return stack;
}

/**
*@brief Verilen iki kelimenin ayni uzunlukta olup olmadigini ve yalnizca bir harf farkinin oldugunu kontrol eder.
*@param First Baslangic ve mevcut kelimenin uzunlugu ayni ise ve kelimeler birbirinden farkli ise her karakteri karsilastir
*@param Second Farkli karakter var ise sayaci bir arttir
*@ return Sayac 1 ise 1 dondurur sayac 1 den farkli ise 0 dondurur
*/
int isWordSuitable(char *startWord, char *currentWord) {
  int diffChar = 0,i;
  if (strlen(startWord) == strlen(currentWord) && strcmp(startWord, currentWord) != 0) {
    for (i = 0; i < strlen(startWord); ++i) {
      if (startWord[i] != currentWord[i]) {
        diffChar++;
      }
    }
  }
  if(diffChar == 1) return 1;
  else return 0;
}

/**
*@brief Verilen kelimenin linkli listede olup olmadigini kontrol eder.
*@param Linkli listede dügümleri gez dügümlerde kelimenin olup olmadigini kontrol et
*@return Eger kelime varsa 1 dondurur yok ise 0 dondurur
*/
int isInUsedWords(Node *head, char *word) {
  while (head != NULL) {
    if (strcmp(head->word, word) == 0) {
      return 1;
    }
    head = head->next;
  }
  return 0;
}


/**
*@brief Dosyadan okunan uygun kelimeleri kuyruga ekler.
*@ param First Dosyayi ac ve okuma modunda basa sarmak icin rewind fonksiyonuyla yeniden konumlandir.
*@param Second Kelime icin bellek ayir ve bu bellegi kullanarak bir dongu baslat.
*@param Third Dosyadan kelimeleri satir satir oku 
*@param Fourth Okunan her kelimenin sonunda yeni satir karakteri varsa, onu null karakteriyle degistir.
*@param Fifth Uygun kelime bulunursa, mevcut stack'in bir kopyasini olustur.
*@param Yeni kelimeyi kopya olan  stack'e ekle.
*@param Yeni bir dugum olustur ve kelimeyi kullanilan kelimeler listesine ekle.
*@ param Kopya olan stack'i kuyruga ekle.
*@param  Bellegi serbest birak ve dosyayi kapat.
*/
void readAndPushWordsToQueue(char *filename, char *startWord, Queue *queue, Stack *current, Node **usedWordsHead) {
	int i;
  FILE *file = fopen(filename, "r");
  rewind(file);
  char *word = (char *)malloc(MAX * sizeof(char));

  while (fgets(word, MAX, file) != NULL) {
    for (i = 0; word[i] != '\0'; i++) {
      if (word[i] == '\n') {
        word[i] = '\0';
      }
    }

    if (isWordSuitable(startWord, word) && !isInUsedWords(*usedWordsHead, word)) {
      Stack *stack = copyStack(current);
      push(stack, word);               

 
      Node *newNode = (Node *)malloc(sizeof(Node));
      newNode->word = malloc(strlen(word) + 1); 
      strcpy(newNode->word, word);
      newNode->next = *usedWordsHead;
      *usedWordsHead = newNode;


      enqueue(queue, stack);  
    }
  }

  free(word);
  fclose(file);
}



/**
*@brief Verilen baslangic kelimesinden baslayarak hedef kelimeye ulasan en kisa yolun bulundugu bir kelime merdiveni olusturan fonksiyon.
*@param Eger kuyruk bossa, baslangic kelimesini iceren bir stack olustur.
*@param Olusturulan stack'i kuyruga ekle.
*@param Kuyruk bos olmadigi surece while dongusu icinde su islemleri yap:
*@param Kuyruktan bir stack cikar ve bu stack'in NULL olup olmadigini kontrol et.
*@param Cikarilan stack'ten mevcut kelimeyi al.
*@param Eger mevcut kelime hedef kelimeye esitse kelimeleri ters sirayla aktarmak icin yeni bir stack olustur,ters siradaki kelimeleri yazdir.
*       Bellekten ayrilmis olan stack'leri serbest birak 
*       @ return 1 dondur.
*@param Eger hedef kelimeye ulasilmadiysa,readAndPushWordsToQueue fonksiyonunu cagir.
*@param Kuyruk bossa ve hedef kelimeye ulasilamamissa, "Yol bulunamadi!" mesajini yazdir
*       @return 0 dondur.
*/

int findPath(char *filename, char *startWord, char *stopWord, Queue *queue, Node **usedWordsHead) {
  if (isQueueEmpty(queue)) {
    Stack *currentStack = createStack();
    push(currentStack, startWord);
    enqueue(queue, currentStack);
  }

  while (!isQueueEmpty(queue)) {
    Stack *currentStack = dequeue(queue);

    if (currentStack == NULL) {
      printf("Hata: Kuyruk boþ!\n");
      return 0;
    }

    char *currentWord = currentStack->item[currentStack->top];

    if (strcmp(currentWord, stopWord) == 0) {
      printf("En kisa yol:\n");

      Stack *reverseStack = createStack();
      while (!isEmpty(currentStack)) {
        push(reverseStack, pop(currentStack));
      }
      
      while (reverseStack->top >0) {
        printf("%s -> ", pop(reverseStack));
      }
      printf("%s\n", pop(reverseStack));

      free(currentStack);
      free(reverseStack);

      return 1; 
    }

    readAndPushWordsToQueue(filename, currentWord, queue, currentStack, usedWordsHead);
  }

  printf("Yol bulunamadi!\n");
  return 0;
}




