json2timetable
==============

Displays a line or station timetable.

Synopsis
--------

```sh
json2timetable timetable.json [station_name]
```

Description
-----------

This program displays the railway line timetable according to the given JSON
file.

When a station name is given as the second argument, it shows the station
timetable.

### About a timetable data file

Timetable files for this program are in the JSON format.

Sample timetable files are supplied in data directory. (data directory is
located in the same directory where this README.md exists.)

- hohi_down.txt, hohi_up.txt
  - The part of Hohi line of Kyushu Railway Company in Japan.

Return Value
------------

Returns 0 when the program exits successfully.

Returns a non-zero value when some error is happened.

---

Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/

