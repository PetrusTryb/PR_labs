#include <windows.h>
#include <stdio.h>
#include<conio.h>
#include <time.h>
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct dane_dla_watku // dane, ktore otrzymaja watki
{
	int* liczby;
	int ile;
};
typedef struct dane_dla_watku dane_dla_watku;
DWORD WINAPI funkcja_watku(void* argumenty);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char** argv)
{
	if(argc<2)
		return 1;
	int data_size = 1024*1024;
	int* data = (int*) malloc(data_size*sizeof(int));
	int N_threads = atoi(argv[1]);
	HANDLE* watki = (HANDLE*) malloc(N_threads*sizeof(HANDLE));
	for (int i = 0; i < data_size; i++)
	{
		data[i] = i;
	}
	DWORD id; // identyfikator watku
	printf(" Uruchomienie programu \n ");
	dane_dla_watku* dane = (dane_dla_watku*) malloc(N_threads*sizeof(dane_dla_watku));
	for (int i = 0; i < N_threads; i++) {
		dane[i].liczby = data + (data_size / N_threads)*i;
		dane[i].ile = (data_size / N_threads);
	}
	clock_t start = clock();
	// tworzenie watkow
	for (int i = 0; i < N_threads; i++)
	{
		watki[i] = CreateThread(
			NULL, // atrybuty bezpieczenstwa
			0, // inicjalna wielkosc stosu
			funkcja_watku, // funkcja watku
			(void*)&dane[i],// dane dla funkcji watku
			0, // flagi utworzenia
			&id);
		if (watki[i] != INVALID_HANDLE_VALUE)
		{
			//printf(" Utworzylem watek %i liczb od adresu %x \n ", dane[i].ile, dane[i].liczby);
			// ustawienie priorytetu
			//SetThreadPriority(watki[i], THREAD_PRIORITY_NORMAL);
		}
	}
	WaitForMultipleObjects(N_threads, watki, 1, INFINITE);
	/*for (int i = 0; i < data_size; i++)
	{
		printf("data[%d] = %d\n", i, data[i]);
	}*/
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Czas dzialania: %f\n", seconds);
	free(data);
	free(watki);
	free(dane);
	return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// takie funkcje pracuja wspolbieznie w programie
DWORD WINAPI funkcja_watku(void* argumenty)
{
	unsigned int licznik = 0;
	// rzutowanie struktury na wlasny wskaznik
	struct dane_dla_watku* moje_dane = (struct dane_dla_watku*)argumenty;
	for (int i = 0; i < moje_dane->ile; i++)
	{
		for(int j = 0; j<1000; j++){
			moje_dane->liczby[i]+=1;
		}
	}
	return 0;
}