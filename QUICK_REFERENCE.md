# EZALLOC - Riferimento Rapido Problemi Critici

## 🔴 PROBLEMI CRITICI DA RISOLVERE IMMEDIATAMENTE

### 1. Use-after-free (srcs/internal/group_handler_internal.c:41-42)
```c
// ❌ SBAGLIATO:
free(group);
group->next = NULL;  // Accesso dopo free!
group->prev = NULL;  // Accesso dopo free!

// ✅ CORRETTO:
free(group);
// Non accedere più a group!
```

### 2. Missing return (srcs/internal/group_handler_internal.c:137)
```c
// ❌ SBAGLIATO:
void *ezg_alloc_handler(...)
{
    // ... codice ...
} // Alcuni path non ritornano nulla!

// ✅ CORRETTO:
void *ezg_alloc_handler(...)
{
    // ... codice ...
    return NULL;  // Aggiungere alla fine
}
```

### 3. Integer overflow in calloc (srcs/ezalloc.c:21, srcs/ezgalloc.c:14)
```c
// ❌ SBAGLIATO:
new_ptr = ez_alloc_handler(size * count, NEW, NO_DATA);

// ✅ CORRETTO:
if (count != 0 && size > SIZE_MAX / count)
    return (NULL);
new_ptr = ez_alloc_handler(size * count, NEW, NO_DATA);
```

### 4. t_garbage non inizializzato (srcs/internal/group_handler_internal.c:72)
```c
// ❌ SBAGLIATO:
group->garbage = NULL;

// ✅ CORRETTO:
group->garbage = malloc(sizeof(t_garbage));
if (!group->garbage) {
    free(group->name);
    free(group);
    return (NULL);
}
group->garbage->head = NULL;
group->garbage->tail = NULL;
```

### 5. Funzione non usata da eliminare (srcs/internal/group_handler_internal.c:4-24)
```c
// ❌ Da ELIMINARE completamente:
static void cleanup_groups_list(t_group **head, t_group **tail)
{
    // ... 21 righe mai chiamate
}
```

---

## 🟠 DUPLICAZIONE CRITICA DA ELIMINARE

### Codice duplicato in ez_calloc e ezg_calloc (16 righe)

**Soluzione:** Creare helper function
```c
// Helper condiviso
static void zero_memory(void *ptr, size_t total_size)
{
    if (!ptr)
        return;
    memset(ptr, 0, total_size);  // Usa memset invece del loop!
}

// Nelle funzioni calloc:
void *ez_calloc(size_t size, size_t count)
{
    // Verifica overflow
    if (count != 0 && size > SIZE_MAX / count)
        return (NULL);
    
    void *new_ptr = ez_alloc_handler(size * count, NEW, NO_DATA);
    zero_memory(new_ptr, size * count);
    return (new_ptr);
}
```

---

## ⏱️ TEMPO STIMATO IMPLEMENTAZIONE

- FASE 1 (Critici): **2-3 ore** ← PRIORITÀ MASSIMA
- FASE 2 (Duplicazioni): **4-6 ore**
- FASE 3 (Organizzazione): **6-8 ore** (opzionale)
- FASE 4 (Polish): **2-3 ore** (opzionale)

**TOTALE MINIMO:** 6-9 ore (FASE 1 + FASE 2)

---

## 📋 CHECKLIST RAPIDA

### Immediato (per far compilare):
- [ ] Rimuovere righe 41-42 da `delete_group()`
- [ ] Aggiungere `return NULL;` a fine `ezg_alloc_handler()`
- [ ] Inizializzare `group->garbage` in `safe_new_group()`
- [ ] Eliminare funzione `cleanup_groups_list()`

### Sicurezza (previene vulnerabilità):
- [ ] Aggiungere check overflow in `ez_calloc()`
- [ ] Aggiungere check overflow in `ezg_calloc()`

### Qualità (riduce duplicazione):
- [ ] Creare funzione `zero_memory()` helper
- [ ] Usare `memset()` invece di loop manuale
- [ ] Unificare logica cleanup liste

---

## 🎯 DOPO LA FASE 1

Il codice:
- ✅ Compilerà senza errori/warning
- ✅ Non avrà use-after-free
- ✅ Non avrà undefined behavior
- ✅ Avrà gruppi funzionanti

---

Per dettagli completi: **IMPROVEMENTS_REPORT.md**
