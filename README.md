# Password Manager

- Tired of forgetting your passwords ? Use a password manager :-)  
- This password manager stores your passwords in an encrypted file and an unlock password is requested to launch the program (and to access your passwords) 

## Important notices

- this program is **not finished**
- this program is **not secure at all**
-> the file containing the name and password combinations and the file containing the unlock password are not encrypted yet 
- some parts of the program may not be compatible with certain standards

## How does the program work ?

- Two void files will be create (if they don't exist yet)
-> "unlock_password.txt" will contain the unlock password
-> "backup.txt" will contain the name and password combinations 

- Step 0 : if the unlock password is not already set, the program asks to set the unlock password
- Step 1 : the program asks to input the unlock password (while the unlock password is not correct)
- Step 2 : the program asks to choose an action : 
-> add a new name/password combination
-> display a password

## Future improvements

- Possibility of changing the values of a combination (name/password)
- Possibility of removing a combination
- Compatibility with all standards
- Asymetric encryption
- Graphic interface
- Forgotten unlock password procedure