import os
import subprocess
import re
import sys
import multiprocessing
import time
import math

class bcolors:
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

n = len(sys.argv)

######## VERSION 1
def version_1():
  print(f'\n{bcolors.FAIL}Current Working Directory:{bcolors.ENDC}' , os.getcwd())

  # Processor Type
  out=subprocess.run(["cat", "/proc/cpuinfo"], capture_output=True).stdout.decode('utf-8').splitlines()
  required_fields=[x for x in out if x.find('model name') != -1]
  print(f'{bcolors.FAIL}Processor type:{bcolors.ENDC}', required_fields[0].split(':')[1].strip())

  # Kernel version
  out=subprocess.run(['cat','/proc/version'],capture_output=True).stdout.decode('utf-8').strip()
  print(f'{bcolors.FAIL}Kernel Version:{bcolors.ENDC}',out)

  # Total Memory 
  out=subprocess.check_output("grep MemTotal /proc/meminfo",shell=True).decode('utf-8').split(':')[1].strip()
  print(f'{bcolors.FAIL}Total Memory:{bcolors.ENDC}', out)

  # Uptime
  out=subprocess.check_output("cat /proc/uptime",shell=True).decode('utf-8').split(' ')[0].strip()
  print(f'{bcolors.FAIL}Uptime:{bcolors.ENDC}', out+' seconds \n')


########## VERSION 2

def read_data(t,result):
  while(True):

    # Processor time
    out=subprocess.check_output("top -n1",shell=True).decode('utf-8')
    required_str=r'\s[0-9]+\.[0-9]\s'
    l = re.findall(required_str,out)[:4]
    del l[2]
    user_percent=float(l[0].strip())
    system_percent=float(l[1].strip())
    idle_percent=float(l[2].strip())
    if user_percent == 0 and system_percent == 0:
      idle_percent=100.0

    # % Free memory
    out=subprocess.check_output("grep MemTotal /proc/meminfo",shell=True).decode('utf-8').split(':')[1].strip()
    mem_total = float(out.split(' ')[0])
    out=subprocess.check_output("grep MemFree /proc/meminfo",shell=True).decode('utf-8').split(':')[1].strip()
    mem_free = float(out.split(' ')[0])
    
    result[0]+=user_percent
    result[1]+=system_percent
    result[2]+=idle_percent
    result[3]+=mem_total
    result[4]+=mem_free
    result[5]+=1 # read operation count
    
    time.sleep(t)
    

def print_data(t, result):
  while(True):
    try:

      div = result[5]

      # Processor states
      print(f'{bcolors.FAIL}Processor: {bcolors.ENDC}' + f'{bcolors.OKGREEN}'+str(result[0]/div)[:6]+'% user ' + f'{bcolors.OKCYAN}'+str(result[1]/div)[:6]+'% system ' + f'{bcolors.OKBLUE}' + str(result[2]/div)[:6] + '% idle' + f'{bcolors.ENDC}')
      
      # Free memory
      print(f'{bcolors.FAIL}Free Memory: {bcolors.ENDC}'+ str(result[4]/div) + ' kB' + f'{bcolors.OKCYAN} ('+ str((result[4]/div)*100/(result[3]/div))[:6] + f'%){bcolors.ENDC}')
      
      # Disk read/write rate
      out = subprocess.check_output("grep sda /proc/diskstats", shell = True).decode('utf-8').strip().split(' ')
      read_sectors = float(out[11])
      write_sectors = float(out[15])
      print(f'{bcolors.FAIL}Disk read rate:{bcolors.ENDC}', str((read_sectors - result[7])/t)+' sectors per sec')
      print(f'{bcolors.FAIL}Disk write rate:{bcolors.ENDC}', str((write_sectors - result[8])/t)+' sectors per sec') 
      result[7] = read_sectors
      result[8] = write_sectors

      # Context switches
      ctxt = subprocess.check_output("grep ctxt /proc/stat", shell = True).decode('utf-8').split(' ')[1]
      print(f'{bcolors.FAIL}Context switch rate: {bcolors.ENDC}'+ str((float(ctxt) - result[9])/t)+' switches per sec')
      result[9] = float(ctxt)

      # Rate of process creation
      out = subprocess.check_output("grep processes /proc/stat",shell=True).decode('utf-8').split(' ')[1].strip()
      processes_created = float(out) # finding out number of processes
      if result[6] != 0:
        print(f'{bcolors.FAIL}Process creation rate:{bcolors.ENDC}', str((processes_created - result[6])/t)+' processes per sec '+ f'({processes_created - result[6]} processes created in {t} seconds)') # printing (current - prev_processes)/t 
      result[6] = processes_created # updating prev_processes

      print('\n')

      result[0]=0.0
      result[1]=0.0
      result[2]=0.0
      result[3]=0.0
      result[4]=0.0
      result[5]=0.0

    except:
      pass
    
    time.sleep(t)

def version_2(t1,t2,result):
  # Processor time %
  p1 = multiprocessing.Process(target=read_data,args=(int(t1),result))
  p2 = multiprocessing.Process(target=print_data,args=(int(t2),result))
  p1.start()
  p2.start()
  p1.join()
  p2.join()   
    
if n == 3:
  read_rate=sys.argv[1]
  out_rate=sys.argv[2]
  result=multiprocessing.Array('f',10)


  version_2(read_rate,out_rate,result)

else:
  version_1()