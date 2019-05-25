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
Untuk compile NATS client
```bash
gcc file.c -L/usr/local/lib -Wl,--as-needed -ldl -lnats -lpthread -lprotobuf -lprotobuf-c -o file
```

## Status Development:
Server Kimochi sudah bisa menerima dan mengirim pesan ke NATS Queue.
![alt text](https://grit.id/github/first_milestone_20190525.png)

Worker sudah bisa mengambil pesan dari NATS Queue.
![alt text](https://grit.id/github/first_milestone_sub_20190525.png)

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