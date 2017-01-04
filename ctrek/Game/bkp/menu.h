#define OPTION_LENGTH 128
/**
 * Selectable menu class
 */
typedef struct Menu {
  int length;
  int highlighted;
  int x;
  int y;
  char **options;
  int *indices;


  ~Menu() {
    destruct();
  }


  void destruct() {
    Debug.log(DEBUG_DEFAULT, "void Menu::destruct()");
    int i;
    for (i = 0; i < length; ++i) {
      free(options[i]);
    }
    if (length > 0) {
      free(options);
      free(indices);
    }
    length = 0;
    highlighted = -1;
  }


  // Allow forward declarations
  Menu() {
    Debug.log(DEBUG_DEFAULT, "Menu::Menu()");
    x = 1;
    y = 1;
    length = 0;
    highlighted = -1;
  }


  Menu(int sx, int sy) {
    Debug.log(DEBUG_DEFAULT, "Menu::Menu(%d, %d)", sx, sy);
    x = sx;
    y = sy;
    length = 0;
    highlighted = -1;
  }

  static Menu *newMenu(int sx, int sy) {
    Debug.log(DEBUG_DEFAULT, "static Menu* Menu::newMenu(%d, %d)", sx, sy);
    Menu *m = (Menu*)malloc(sizeof(Menu));
    m->x = sx;
    m->y = sy;
    m->length = 0;
    m->highlighted = -1;
    return m;
  }


  static Menu *load(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "static Menu* Menu::load(%d)", fp);
    Menu *m = newMenu(0, 0);
    fread(m, sizeof(Menu), 1, fp);
    fread(&(m->length), sizeof(int), 1, fp);
    m->options = (char**)malloc(m->length * sizeof(char*));
    m->indices = (int*)malloc(m->length * sizeof(int));
    int i;
    for (i = 0; i < m->length; i++) {
      m->options[i] = (char*)malloc(OPTION_LENGTH * sizeof(char));
      fread(m->options[i], OPTION_LENGTH, 1, fp);
      fread(&(m->indices[i]), sizeof(int), 1, fp);
    }
    return m;
  }

  int save(FILE *fp) {
    Debug.log(DEBUG_DEFAULT, "int Menu::save(%d)", fp);
    fwrite(this, sizeof(Menu), 1, fp);
    fwrite(&length, sizeof(int), 1, fp);
    int i;
    for (i = 0; i < length; i++) {
      fwrite(options[i], OPTION_LENGTH, 1, fp);
      fwrite(&indices[i], sizeof(int), 1, fp);
    }

    return 1;
  }

  static void unload(Menu *m) {
    Debug.log(DEBUG_DEFAULT, "static void Menu::unload(%d)", m);
    if (m == NULL) {
      return;
    }
    int i;
    for (i = 0; i < m->length; i++) {
      free(m->options[i]);
    }

    if (m->length > 0) {
      free(m->options);
      free(m->indices);
    }
    free(m);
    return;
  }


  int addOption(const char *format, ...) {
    char buffer[1024] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BUFFER_MEDIUM, format, args);
    va_end(args);
    Debug.log(DEBUG_DEFAULT, "int Menu::addOption(%s, %s)", format, buffer);

    return addOption(buffer);
  }


  int addOption(char *i) {
    Debug.log(DEBUG_DEFAULT, "int Menu::addOption(%s)", i);
    if (length == 0) {
      options = (char**)malloc(sizeof(char*));
      indices = (int*)malloc(sizeof(int));
    } else {
      options = (char**)realloc(options, (length + 1) * sizeof(char*));
      indices = (int*)realloc(indices, (length + 1 ) * sizeof(int));
    }

    options[length] = (char*)malloc(OPTION_LENGTH * sizeof(char));
    strcpy(options[length], i);
    length++;
    return length;
  }


  int isHighlighted(const char *searchStr) {
    Debug.log(DEBUG_DEFAULT, "int Menu::isHighlighted(%s)", searchStr);
    return (strstr(options[highlighted], searchStr) != NULL);
  }

  int optionIndex(const char *searchStr) {
    Debug.log(DEBUG_DEFAULT, "int Menu::optionIndex(%s)", searchStr);
    int i;
    for (i = 0; i < length; ++i) {
      if (strstr(options[i], searchStr) != NULL) {
        return i;
      }
    }

    return -1;
  }

  int getSelection() {
    int choice = -1;
    while (choice == -1) {
      choice = getInput();
    }
    return choice;
  }

  int getInput() {
    int i, choice = -1;

    if (highlighted < 0 || highlighted >= length) {
      highlighted = 0;
    }

    for (i = 0; i < length; i++) {
      gotoXY(x, y + i);
      if (i == highlighted) {
        printf(STR_COL_INVERT "%s" STR_COL_RESET, options[i]);
      } else {
        printf("%s", options[i]);
      }
    }

    choice = getch_arrow();
    Debug.log(DEBUG_DEFAULT, "\n\nmenu choice: %c", choice);
    if (choice == KEY_UP || choice == KEY_WARP_UP) {
      highlighted = (highlighted + length - 1) % length;
      return -1;
    } else if (choice == KEY_DOWN || choice == KEY_WARP_DOWN) {
      highlighted = (highlighted + 1) % length;
      return -1;
    } else if (choice == KEY_RETURN) {
      return KEY_RETURN;
    } else if (choice - '0' >= 0 && choice - '0' <= length - 1) {
      highlighted = choice - '0';
      return highlighted;
    } else if (choice == KEY_HELP) {
      return KEY_HELP;
    }

    return -1;
  }

} Menu;

