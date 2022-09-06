# Pseudomalloc in DisastrOS

## Cosa ho fatto
Ho implementato una pseudomalloc per DisastrOS, a questo scopo ho:

- Implementato un buddyallocator con il solo utilizzo di bitmap.
- Aggiunto nei PCB 1MB di memoria da gestire con l'allocatore.
- Aggiunto nei PCB buffer di gestione delle bitmap.
- Implementato le syscall DisastrOS_malloc e DistastrOS_free per permettere ai processi di utilizzare la nuova pseudo malloc implementata. Esse richiamo le funzioni BuddyAllocator_malloc e BuddyAllocator_free dopo aver inizializzato l'allocatore globale con i buffer sopracitati del processo running.

## Perchè
A DisastrOS mancava una malloc, utile nel sistema operativo per permettere ai processi di allocare memoria dinamicamente.

## How to run
Nel terminale:
```console
cd Buddy_allocator
make
cd ..
make
./disastrOS_test
```
