import sympy as sp
from sympy.solvers import solve
 
Rm, Rs, Rr, Vr, Vb, Vm = sp.symbols('Rm, Rs, Rr, Vr, Vb, Vm')
solution = solve((Vm/(Rs + Rm)) - ((Vr - Vb)/(Rm + Rs +Rr)), Rm)  
print(solution[0])
