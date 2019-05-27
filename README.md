# Kimochi
## Indonesian Open Source Secure Chat Platform

### Dependencies:
C Libraries/ Binaries Distributions
```bash
kore lnats lpthread ldl
```
Untuk nats-streaming-server:
```bash
libprotobuf-dev  libtool autoconf automake
```

### Development Notes:
Untuk compile Worker
```bash
gcc subq1.c natshighsend.c redishigh.c  -L/usr/local/lib -L. -I/usr/local/include -I/usr/local/include/hiredis  -Wl,--as-needed -ldl -lnats -lpthread -lprotobuf -ljson-c -lhiredis -o worker1
```
## Architecture
![alt text](https://grit.id/github/kimochi-arch.png)
## Status Development:
Server Kimochi sudah bisa menerima pesan, meroutingnya, dan client sudah bisa mengambil pesan
dari pengirim.
![alt text](https://grit.id/github/first_milestone_20190527.png)

Worker sudah menjalankan fungsinya sebagai router.
![alt text](https://grit.id/github/first_milestone_sub_20190527.png)

## Milestones Pengembangan (dalam progress)
- Bisa menerima pesan
- Bisa mengoper pesan ke MQ1
- Worker bisa mengambil pesan dari MQ1 lempar ke MQ2
- User bisa mengambil pesan dari user lain yang mengirim
- Database user untuk registrasi, login dan daftar kontak
- Security: validasi dan enkripsi baik transport HTTPS dan cipher
- Front-end berupa web dan mobile app

## Mengapa menggunakan C?
Karena kami bisa. C juga memiliki performa tinggi dengan machine code yang sedikit.

## Anda ingin berkontribusi?
Anda bisa cari dan chat kami di Grup telegram: t.me/idcplc

## License
Dibuat di bawah lisensi MIT.

Inisiasi dilakukan oleh teman-teman dari grit.id . Dimulai dari hal kecil di Indonesia, untuk dunia.