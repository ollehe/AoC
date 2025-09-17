import pprint as p
import re

categories = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]


def generate_passports(string):
    passports =[]
    with open(string) as f: 
        lines = f.readlines()
        passport_string =""
        #read all lines
        for line in lines : 
            stripped = line.strip("\n")
            passport_string += stripped+" "
            # when we encounter an empty line we create a passport defined as 
            # a dictionary from the resulting string. 
            if not stripped : 
                passport = {}
                raw_categories = [cat for cat in passport_string.split(' ') if cat]
                # parse the raw string of form xyz:blabla 
                # and add it to dict[xyz]=blabla
                for raw_cat in raw_categories:
                    passport[ raw_cat.split(':')[0]  ] = raw_cat.split(':')[1]
                passports.append(passport)
                passport_string=""
        # if the file doesn't end with a blank space this 
        # takes care of the last passport
        if passport_string.strip() : 
            passport = {}
            raw_categories = [cat for cat in passport_string.split(' ') if cat]
            for raw_cat in raw_categories:
                passport[ raw_cat.split(':')[0]  ] = raw_cat.split(':')[1]
            passports.append(passport)
    return passports


def is_valid(passport) : 
    if len(passport) == 8 or (len(passport) == 7 and 'cid' not in passport) : 
        if not ( passport["byr"].isdigit() and \
                int(passport["byr"])>=1920 and int(passport["byr"])<=2002 ) :
            return False
        if not ( passport["iyr"].isdigit() and \
                int(passport["iyr"])>=2010 and int(passport["iyr"])<=2020 ):
            return False 
        if not ( passport["eyr"].isdigit() and \
                int(passport["eyr"])>=2020 and int(passport["eyr"])<=2030 ):
            return False 
        pattern = r"^(1([5-8][0-9]|9[0-3])cm|([5-6][0-9]|7[0-6])in)$"
        if not re.match(pattern, passport["hgt"]) :
            return False
        pattern = r"^#[0-9a-f]{6}$"
        if not re.match(pattern,passport["hcl"]) :
            return False
        if not ( passport["ecl"] in ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"] ):
            return False 
        pattern = r"[0-9]{9}$"
        if not re.match(pattern,passport["pid"]) : 
            return False
        return True
    return False

# Assumes input is a list of dictionaries 
def validate_passports(passports) : 
    number_of_valid_passports = 0
    for passport in passports : 
        if is_valid(passport) :
            # p.pprint(passport)
            # print("is valid")
            number_of_valid_passports+=1
        # else : 
        #     p.pprint(passport)
        #     print("isn't valid")
    return number_of_valid_passports

passports = generate_passports("Day4.txt")
print(validate_passports(passports))
