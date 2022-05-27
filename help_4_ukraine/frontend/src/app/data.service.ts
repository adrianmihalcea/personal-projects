import { Injectable } from '@angular/core';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor, HttpClient, HttpHeaders, HttpParamsOptions, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  private url: string = "http://localhost:4200/api";

  constructor(private http:HttpClient) { }

  // hello() {
  //   console.log('hello!');
  // }

  //locations, viewAll ok

  //daca e admin pe locatia respectiva ok

  //ca rol, daca e admin

  //lista cu iteme si nr de unitati pentru locatie cu id

  //post cu donatia

  //post cu adauga item

  getHubs(): Observable<any> {
    const mheaders = new HttpHeaders({
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${localStorage.getItem('token')}`
    })

    return this.http.get(this.url + "/locations/viewAll", {headers: mheaders});
  }

  getHubItems(id): Observable<any> {
    const mheaders = new HttpHeaders({
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${localStorage.getItem('token')}`
    })

    const role = localStorage.getItem('role');
    const myObject: any = { locationId: id, role: role};
    const httpParams: HttpParamsOptions = { fromObject: myObject } as HttpParamsOptions;

    const options = { params: new HttpParams(httpParams), headers: mheaders };

    return this.http.get(this.url + "/requests/view", options);
  }

  getHubHistory(id): Observable<any> {
    const mheaders = new HttpHeaders({
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${localStorage.getItem('token')}`
    })

    const myObject: any = { locationId: id};
    const httpParams: HttpParamsOptions = { fromObject: myObject } as HttpParamsOptions;

    const options = { params: new HttpParams(httpParams), headers: mheaders };

    return this.http.get(this.url + "/locations/history", options);
  }

  addItem(id, iname, icount, ipriority): Observable<any> {
    const mheaders = new HttpHeaders({
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${localStorage.getItem('token')}`
    })

    const myObject: any = { locationId: id};
    const httpParams: HttpParamsOptions = { fromObject: myObject } as HttpParamsOptions;

    const options = { params: new HttpParams(httpParams), headers: mheaders };

    return this.http.post(this.url + "/requests/add", JSON.stringify({name: iname, count: icount, priority: ipriority}), options);
  }

  addDonation(id, reqs): Observable<any> {
    const mheaders = new HttpHeaders({
      'Content-Type': 'application/json',
      'Authorization': `Bearer ${localStorage.getItem('token')}`
    })

    const myObject: any = { locationId: id};
    const httpParams: HttpParamsOptions = { fromObject: myObject } as HttpParamsOptions;

    const options = { params: new HttpParams(httpParams), headers: mheaders };

    return this.http.post(this.url + "/requests/received", reqs, options);
  }

}
