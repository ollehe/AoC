reports = []

with open("input.txt") as f : 
    lines = f.readlines()
    for line in lines : 
        reports.append([int(val) for val in line.split() ])


def diff(report) : 
    return [report[i]-report[i+1] for i in range(len(report)-1) ]

def check_validity(report) : 
    increments = diff(report) 
    if len(increments) == 1 and abs(increments[0]) <=3 and increments[0] >= 1 : 
        return True

    return all( val > 0 and val <= 3 for val in increments ) or all( -val > 0 and -val <= 3 for val in increments )


valid = 0
part2 = True

for report in reports : 
    valid_report = check_validity(report)
    if valid_report : 
        valid += 1
    elif part2 and not valid_report :
        print(report, "is not valid")
        modified = []
        for pos in range(len(report)) : 
            modified = report.copy() 
            modified.pop(pos)
            if check_validity(modified) : 
                print(report, "is valid by removing element", pos)
                valid +=1
                break
        print("Cannot modify report to make it valid")

print("Valid reports",valid)
