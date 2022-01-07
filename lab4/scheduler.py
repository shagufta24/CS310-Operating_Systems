def process_input():
    algorithm=input()
    n=input()
    n=int(n)
    buffer=input()
    arrival_times=[int(x) for x in buffer.split()]
    buffer=input()
    burst_times=[int(x) for x in buffer.split()]
    if algorithm == '4':
        buffer=input()
        priorities=[int(x) for x in buffer.split()]
        return (algorithm, n, arrival_times, burst_times, priorities)
    elif algorithm == '5':
        buffer=input()
        quant=int(buffer)
        return (algorithm, n, arrival_times, burst_times, quant)
    return (algorithm, n, arrival_times, burst_times)
                
def fcfs(arrival_times, burst_times,n):
    sorted_burst_times=[x for _,x in sorted(zip(arrival_times, burst_times), key = lambda x:x[0])]
    sorted_arrival_times=sorted(arrival_times)
    original_indices = [idx for idx,_ in sorted(enumerate(arrival_times), key=lambda x:x[1])]
    current_time=sorted_arrival_times[0]
    execution_start_times=[]
    for i in range(n):
        if current_time < sorted_arrival_times[i]:
            current_time = sorted_arrival_times[i]
        execution_start_times.append(current_time)
        current_time+=sorted_burst_times[i]
    waiting_times=[x-y for x,y,z in zip(execution_start_times,sorted_arrival_times, sorted_burst_times)]
    response_times=[x-y for x,y in zip(execution_start_times, sorted_arrival_times)]
    turnaround_times=[x-y+z for x,y,z in zip(execution_start_times,sorted_arrival_times, sorted_burst_times)]

    final_waiting_times=[x for _,x in sorted(zip(original_indices, waiting_times))]
    final_response_times=[x for _,x in sorted(zip(original_indices, response_times))]
    final_turnaround_times=[x for _,x in sorted(zip(original_indices, turnaround_times))]

    print('\n')
    print(*final_waiting_times)
    print(*final_response_times)
    print(*final_turnaround_times)
    throughput = n/current_time
    format_throughput = "{:.4f}".format(throughput)
    print(format_throughput)

def sjf(arrival_times, burst_times,n):
    sorted_burst_times=sorted(burst_times)
    # print(sorted_burst_times)
    original_indices = [idx for idx,_ in sorted(enumerate(burst_times), key=lambda x:x[1])]
    sorted_arrival_times=[x for _,x in sorted(zip(burst_times, arrival_times))]
    # print(sorted_arrival_times)
    flag = [0] * n
    flag_count = 0
    execution_start_times=[9999]*n

    #executing first process (which arrived first)
    # min_arrival_time = min(arrival_times)
    # min_index= arrival_times.index(min_arrival_time)
    # current_time=min_arrival_time
    # execution_start_times.append(current_time)
    # current_time += burst_times[min_index]
    # flag[min_index] = 1
    # flag_count += 1

    current_time = 0

    #executing remaining processes
    while flag_count < n: 
        for i in range(n):
            if flag[i] == 0:
                if current_time < sorted_arrival_times[i]:
                    continue
                else:
                    # current_time = sorted_arrival_times[i]
                    execution_start_times[i] = current_time
                    current_time+=sorted_burst_times[i]
                    # print(i, current_time)
                    # print(execution_start_times)
                    flag[i] = 1
                    flag_count += 1

    waiting_times=[x-y for x,y,z in zip(execution_start_times,sorted_arrival_times, sorted_burst_times)]
    response_times=[x-y for x,y in zip(execution_start_times, sorted_arrival_times)]
    turnaround_times=[x-y+z for x,y,z in zip(execution_start_times,sorted_arrival_times, sorted_burst_times)]

    final_waiting_times=[x for _,x in sorted(zip(original_indices, waiting_times))]
    final_response_times=[x for _,x in sorted(zip(original_indices, response_times))]
    final_turnaround_times=[x for _,x in sorted(zip(original_indices, turnaround_times))]

    print('\n')
    print(*final_waiting_times)
    print(*final_response_times)
    print(*final_turnaround_times)
    throughput = n/current_time
    format_throughput = "{:.4f}".format(throughput)
    print(format_throughput)

def srtf(arrival_times, burst_times,n):
    sorted_burst_times=[x for _,x in sorted(zip(arrival_times, burst_times))]
    sorted_arrival_times=sorted(arrival_times)
    start_flag=[0]*n
    response_flag=[0]*n
    remaining_times=[999]*n
    response_times=[999]*n
    waiting_times=[0]*n
    completed_processes=0
    is_completed=[0]*n
    idx=-1
    for t in range(sorted_arrival_times[0], 9999):
        arrived=[idx for idx,x in enumerate(sorted_arrival_times) if x<=t]
        arrived=[x for x in arrived if is_completed[x]==0]
        # print('Arrived processes', arrived)
        for i in arrived:
            if start_flag[i] == 0:
                # print('New Process')
                remaining_times[i] = sorted_burst_times[i]
                start_flag[i]=1
        arrived_bursts=[remaining_times[i] for i in arrived]
        arrived=[x for _,x in sorted(zip(arrived_bursts, arrived))]
        
        idx=arrived[0]
        if response_flag[idx]==0:
            response_times[idx]=t
            response_flag[idx]=1
            # print('Response times', response_times)
        # print('Executing process', idx)
        
        remaining_times[idx]-=1
        # print('Remaining times', remaining_times)
        waiting_indices=arrived[1:]
        for i in waiting_indices:
            waiting_times[i]+=1
        # print('Waiting times', waiting_times)
        t+=1
        if remaining_times[idx]==0:
            is_completed[idx]=1
            # print('completed array',is_completed)
            # print('Completed process', idx)
            completed_processes+=1
        if completed_processes==n:
            break

    print('\n')    
    print(*waiting_times)
    print(*[x-y for x,y in zip(response_times, arrival_times)])
    print(*[x+y for x,y in zip(waiting_times, sorted_burst_times)])
    throughput = n/t
    format_throughput = "{:.4f}".format(throughput)
    print(format_throughput)

def priority(arrival_times, burst_times, priorities, n):
    sorted_burst_times=[x for _,x in sorted(zip(arrival_times, burst_times), key=lambda x:x[0])]
    sorted_arrival_times=sorted(arrival_times)
    original_indices = [idx for idx,_ in sorted(enumerate(arrival_times), key=lambda x:x[1])]
    # print('sat',sorted_arrival_times)
    # print('sbt',sorted_burst_times)
    start_flag=[0]*n
    response_flag=[0]*n
    remaining_times=[999]*n
    response_times=[999]*n
    waiting_times=[0]*n
    completed_processes=0
    is_completed=[0]*n
    for t in range(sorted_arrival_times[0], 9999):
        arrived=[idx for idx,x in enumerate(sorted_arrival_times) if x<=t]
        arrived=[x for x in arrived if is_completed[x]==0]
        # print('Arrived processes', arrived)
        for i in arrived:
            if start_flag[i] == 0:
                # print('New Process')
                remaining_times[i] = sorted_burst_times[i]
                start_flag[i]=1
        arrived_priorities=[priorities[i] for i in arrived]
        arrived=[x for _,x in sorted(zip(arrived_priorities, arrived))]
        
        idx=arrived[0]
        if response_flag[idx]==0:
            response_times[idx]=t
            response_flag[idx]=1
        #     print('Response times', response_times)
        # print('Executing process', idx)
        
        remaining_times[idx]-=1
        # print('Remaining times', remaining_times)
        waiting_indices=arrived[1:]
        for i in waiting_indices:
            waiting_times[i]+=1
        # print('Waiting times', waiting_times)
        t+=1
        if remaining_times[idx]==0:
            is_completed[idx]=1
            # print('completed array',is_completed)
            # print('Completed process', idx)
            completed_processes+=1
        if completed_processes==n:
            break

    final_waiting_times=[x for _,x in sorted(zip(original_indices, waiting_times))]
    final_response_times=[x for _,x in sorted(zip(original_indices, response_times))]
    turnaround_times = [x+y for x,y in zip(waiting_times, sorted_burst_times)]
    final_turnaround_times=[x for _,x in sorted(zip(original_indices, turnaround_times))]

    print('\n')
    print(*final_waiting_times)
    print(*final_response_times)
    print(*final_turnaround_times) 
    # print('\n')
    # print(*waiting_times)
    # print(*[x-y for x,y in zip(response_times, arrival_times)])
    # print(*[x+y for x,y in zip(waiting_times, sorted_burst_times)])
    throughput = n/t
    format_throughput = "{:.4f}".format(throughput)
    print(format_throughput)

def round_robin(arrival_times, burst_times, quant, n):
    sorted_burst_times=[x for _,x in sorted(zip(arrival_times, burst_times), key=lambda x:x[0])]
    sorted_arrival_times=sorted(arrival_times)
    original_indices = [idx for idx,_ in sorted(enumerate(arrival_times), key=lambda x:x[1])]
    # print('sat',sorted_arrival_times)
    # print('sbt',sorted_burst_times)
    start_flag=[0]*n
    response_flag=[0]*n
    remaining_times=[999]*n
    response_times=[999]*n
    waiting_times=[0]*n
    completed_processes=0
    is_completed=[0]*n
    queue=[]
    counter=0
    for t in range(sorted_arrival_times[0], 9999):
        # print('\n')
        arrived=[idx for idx,x in enumerate(sorted_arrival_times) if x<=t]
        arrived=[x for x in arrived if is_completed[x]==0]
        # print('a', arrived)
        for x in arrived:
            if x not in queue:
                queue.append(x)
        # print('Arrived processes', queue)
        for i in arrived:
            if start_flag[i] == 0:
                # print('New Process')
                remaining_times[i] = sorted_burst_times[i]
                start_flag[i]=1
        if counter%quant==0 and counter != 0:
            queue.pop(0)
        try:
            idx=queue[0]
        except:
            continue
        # print('Executing process', idx)
        if response_flag[idx]==0:
            response_times[idx]=t
            response_flag[idx]=1
            # print('Response times', response_times)
        
        
        remaining_times[idx]-=1
        # print('Remaining times', remaining_times)
        waiting_indices=[x for x in arrived if x != idx and remaining_times[x] != 0]
        for i in waiting_indices:
            waiting_times[i]+=1
        # print('Waiting times', waiting_times)
        t+=1
        counter+=1
        if remaining_times[idx]==0:
            is_completed[idx]=1
            # print('completed array',is_completed)
            # print('Completed process', idx)
            completed_processes+=1
            counter=0
            queue.pop(0)
        if completed_processes==n:
            
            break

    final_waiting_times=[x for _,x in sorted(zip(original_indices, waiting_times))]
    final_response_times=[x for _,x in sorted(zip(original_indices, response_times))]
    turnaround_times = [x+y for x,y in zip(waiting_times, sorted_burst_times)]
    final_turnaround_times=[x for _,x in sorted(zip(original_indices, turnaround_times))]

    print('\n')
    print(*final_waiting_times)
    print(*final_response_times)
    print(*final_turnaround_times)
    # print(*waiting_times)
    # print(*[x-y for x,y in zip(response_times, arrival_times)])
    # print(*[x+y for x,y in zip(waiting_times, sorted_burst_times)])
    throughput = n/t
    format_throughput = "{:.4f}".format(throughput)
    print(format_throughput)

if __name__ == "__main__":

    response = process_input()
    if response[0] == '1':
        fcfs(response[2], response[3], int(response[1]))
    elif response[0] == '2':
        sjf(response[2], response[3], int(response[1]))
    elif response[0] == '3':
        srtf(response[2], response[3], int(response[1]))
    elif response[0] == '4':
        priority(response[2], response[3], response[4], int(response[1]))
    elif response[0] == '5':
        round_robin(response[2], response[3], int(response[4]), int(response[1]))

    