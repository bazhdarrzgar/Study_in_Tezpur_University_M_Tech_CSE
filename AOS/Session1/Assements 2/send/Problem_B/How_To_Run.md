## How to Run


```bash
./proxy_server 8000
```

- Access designated `file1.txt`:
```bash
./client 127.0.0.1 8000 file1.txt
```
- Access designated `file2.txt`:
```bash
./client 127.0.0.1 8000 file2.txt
```
- Request a non-designated file (error handling):
```bash
./client 127.0.0.1 8000 unknown.txt
```
- Access files concurrently:
```bash
./client 127.0.0.1 8000 file1.txt file2.txt file1.txt file2.txt
```


