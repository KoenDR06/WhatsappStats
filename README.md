## android\_metadata

### General
Stores metadata about the device

### Columns
- `locale`: TEXT

### Keys

---

## audio\_data

### General
Stores waveform data of audio messages. The big thing that it stores is in the waveform column, where it stores the waveform data in 128 hexadecimal characters, which makes me believe it's encrypted or hashed.

### Columns
- `message_row_id`:                     INTEGER
- `waveform`:                           BLOB
- `background-color`:                   INTEGER = 0
- `transcription_status`:               INTEGER
- `transcription_locale`:               INTEGER
- `transcription_confidence_threshold`: INTEGER
- `transcription_request_locale`:       INTEGER
- `transcription_feedback_submitted`:   INTEGER
- `transcription_id`:                   TEXT

### Keys
- `message_row_id`

---

## backup\_changes

### General
Stores what changes happened to what backup table? Idk this one confuses me and seems irrelevant

### Columns
- `_id`:            INTEGER autoIncrement
- `operation`:      TEXT
- `table_name`:     TEXT
- `table_row_id`:   INTEGER

### Keys
- `_id`

---

## call\_log

### Columns

