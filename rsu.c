void setAlert(int idc)
{
    RSUAnomaly[idR].Vx = I[idR].Px;
    RSUAnomaly[idR].Vy = I[idR].Py; 
    RSUAnomaly[idR].VxA = Mgen[idc].VxA;
    RSUAnomaly[idR].VyA = Mgen[idc].VyA;
}
