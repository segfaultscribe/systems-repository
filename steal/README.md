# A simple `cp` from scratch: File copying utility

Two variants:
- `steal` : normal batch copier 
- `threaded_steal` : bacth copying with multithreading

### Usage

```
# steal
gcc steal.c -o s
./s file1.txt file2.txt file3.txt destination_folder

# threaded steal
gcc threaded_steal -o ts
./ts file1.txt file2.txt file3.txt destination_folder
```

A good idea to work on afterwards would be recursive file copier from one directory to another. Happy coding! ✌🏼  
