# Vaccine Batch Management System

This was a project for my Introduction to Algorithms and Data Structures class. It is a program written in C to manage  
vaccine inoculations to users, allowing registration of different vaccine batches and logging their administration and availability.


## Usage

To use the Vaccine Batch Management System, run the compiled program:

```bash
./vaccinesystem
```

You can also run the program in Portuguese mode with:
```bash
./vaccinesystem pt
```

### Commands

1. **Quit the program (`q`):**
   - **Input:** `q`
   - **Output:** *(none)*

2. **Create a new vaccine batch (`c`):**
   - **Input:** `c <batch> <day>-<month>-<year> <number-of-doses> <vaccine-name>`
   - **Output:** `<batch>`
   - **Errors:**
     - `too many vaccines`
     - `duplicate batch number`
     - `invalid batch`
     - `invalid name`
     - `invalid date`
     - `invalid quantity`

3. **List batches (`l`):**
   - **Input:** `l [<vaccine-name> { <vaccine-name> } ]`
   - **Output:** One line per batch in the format  
     `<vaccine-name> <batch> <day>-<month>-<year> <available-doses> <applications>`
   - **Errors:**
     - `<vaccine-name>: no such vaccine`

4. **Apply a dose to a user (`a`):**
   - **Input:** `a <user-name> <vaccine-name>`
   - **Output:** `<batch>` (the batch used)
   - **Errors:**
     - `no stock`
     - `already vaccinated`

5. **Remove a batch (`r`):**
   - **Input:** `r <batch>`
   - **Output:** `<number-of-doses-applied>`
   - **Errors:**
     - `<batch>: no such batch`

6. **Delete user application record(s) (`d`):**
   - **Input:** `d <user-name> [<date> [<batch>]]`
   - **Output:** `<number-of-deleted-applications>`
   - **Errors:**
     - `<user-name>: no such user`
     - `invalid date`
     - `<batch>: no such batch`

7. **List vaccine applications (`u`):**
   - **Input:** `u [<user-name>]`
   - **Output:**  
     `<user-name> <batch> <day>-<month>-<year>` (for each application)
   - **Errors:**
     - `<user-name>: no such user`

8. **Advance simulated date (`t`):**
   - **Input:** `t [<day>-<month>-<year>]`
   - **Output:** New date (or current date if no argument is provided)
   - **Errors:**
     - `invalid date`

## Notes

- Initial system date is `01-01-2025`.
- Commands and errors adapt to Portuguese if the program is launched with `pt` as an argument.
- The program supports validation for batch naming conventions, date correctness, and prevents invalid or duplicate operations.

## Grade

- **61/64 private tests passed**  
- **17.25 / 20.00**
