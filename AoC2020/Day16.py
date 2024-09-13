from math import prod
import re
import pprint


def parse_input(file_name):
    intervals = []
    my_ticket = []
    nearby_tickets = []
    pattern = r'\d+'
    index = 0
    with open(file_name) as file:
        lines = file.readlines()
        while index < len(lines):
            line = lines[index]
            if 'or' in line:
                matches = re.findall(pattern, line)
                first_interval = range(int(matches[0]), int(matches[1]) + 1)
                second_interval = range(int(matches[2]), int(matches[3]) + 1)
                category = line.split(':')[0]
                intervals.append([category, first_interval, second_interval])

            if 'your' in line:
                index += 1
                line = lines[index]
                my_ticket = re.findall(pattern, line)
                my_ticket = [int(val) for val in my_ticket]
            if 'nearby' in line:
                index += 1
                while index < len(lines):
                    line = lines[index]
                    matches = re.findall(pattern, line)
                    nearby_tickets.append([int(val) for val in matches])
                    index += 1
            index += 1
        return intervals, my_ticket, nearby_tickets


def is_contained(list_of_intervals, value):
    # list of intervals is of the form [ ['string', range , range ],...] so
    # we want to keep the ranges
    intervals_wo_cats = [
        val for cat in list_of_intervals for val in cat
        if isinstance(val, range)
    ]
    for interval in intervals_wo_cats:
        if value in interval:
            return True
    return False


def is_valid(list_of_intervals, ticket):
    for value in ticket:
        if not is_contained(list_of_intervals, value):
            return False
    return True


def compute_ticket_scanning_error_rate(list_of_intervals, nearby_tickets):
    error_rate = 0
    unpacked = [value for ticket in nearby_tickets for value in ticket]
    for value in unpacked:
        if not is_contained(list_of_intervals, value):
            error_rate += value
    return error_rate


def get_valid_tickets(list_of_intervals, nearby_tickets):
    valid_tickets = []
    for ticket in nearby_tickets:
        if is_valid(list_of_intervals, ticket):
            valid_tickets.append(ticket)
    return valid_tickets


def determine_potential_categories(list_of_intervals, valid_tickets):
	cat_map = dict()
	for cat in list_of_intervals : 
		cat_map[cat[0]] = []	
	for col in range(len(valid_tickets[0])):
		col_vals = [ valid_tickets[row][col] for row in range(len(valid_tickets)) ]
		for cat in list_of_intervals:
			all_contained = True
			for val in col_vals:
				all_contained = all_contained and (val in cat[1] or val in cat[2])
			if all_contained :
				cat_map[cat[0]].append(col)
	return cat_map

def reduce_categories(category_map): 
	reduced_cat_map = dict()
	for key in category_map : 
		reduced_cat_map[key] = []
	reduced = False
	updated_cat_map = category_map
	while not reduced :
		next_cat_map = updated_cat_map
		for key in updated_cat_map : 
			if len(updated_cat_map[key]) == 1 : 
				val = updated_cat_map[key][0]
				reduced_cat_map[key] = val 
				for other_key in updated_cat_map :
					if val in updated_cat_map[other_key] : 
						next_cat_map[other_key].remove(val)
				continue
		updated_cat_map = next_cat_map
		reduced =  sum( [len(updated_cat_map[key]) for key in updated_cat_map ]  ) == 0
	return reduced_cat_map				
		


intervals, my_ticket, nearby_tickets = parse_input('input.txt')

print("Error rate: ",
      compute_ticket_scanning_error_rate(intervals, nearby_tickets))
valid_tickets = get_valid_tickets(intervals, nearby_tickets)

cat_map = determine_potential_categories(intervals, valid_tickets)
reduced_cat_map = reduce_categories(cat_map)
pprint.pprint(reduced_cat_map)
indices = [reduced_cat_map[key] for key in reduced_cat_map if 'departure' in key]
print("Product of categories containing departure:", prod( [my_ticket[val] for val in indices] ) ) 