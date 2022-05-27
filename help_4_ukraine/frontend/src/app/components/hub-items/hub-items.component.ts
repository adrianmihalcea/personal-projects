import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { DataService } from 'src/app/data.service';

@Component({
  selector: 'app-hub-items',
  templateUrl: './hub-items.component.html',
  styleUrls: ['./hub-items.component.css']
})
export class HubItemsComponent implements OnInit {

  isAdmin: boolean;
  donationErr: boolean = false;
  additionErr: boolean = false;
  itemInput: string = null;
  amountInput: number = null;
  priorityInput: number = null;
  items = [];
  amounts = [];
  priorities = [];
  history = [];

  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private data: DataService
  ) { }

  ngOnInit(): void {
    if (!localStorage.getItem('token')) {
      this.router.navigate(['/']);
    }
    
    this.donationErr = false;
    if (localStorage.getItem('role') == 'admin') {
      this.isAdmin = true;
    }

    console.log('hub: ', localStorage.getItem('hub'));
    this.data.getHubItems(localStorage.getItem('hub')).subscribe(it => {
      for (let item of it['requests']) {
        this.items.push([item['name'], item['count'], item['priority']]);
      }
    })

    this.data.getHubHistory(localStorage.getItem('hub')).subscribe(it => {
      for (let item of it) {
        this.history.push([item['itemName'], item['count'], item['donatedBy'], item['timestamp']]);
      }
    })
  }

  // items = [['ananas', 1], ['bere', 100], ['jucarii', 50]]
  // amounts = [0, 0, 0]

  delete(i) {
    this.items.splice(i, 1);
  }

  addItem() {
    if (this.itemInput && this.amountInput > 0 && this.priorityInput != null) {
      this.items.push([this.itemInput, this.amountInput, this.priorityInput]);
      const obj = this.data.addItem(localStorage.getItem('hub'), this.itemInput, this.amountInput, this.priorityInput);
      console.log(obj);
      obj.subscribe (ans => console.log(ans));
      this.additionErr = false;
    } else {
      this.additionErr = true;
    }
    this.itemInput = null;
    this.amountInput = null;
    this.priorityInput = null;

    if (!this.additionErr) {
      this.router.navigate(['/hubs']);
    } else {
      console.log('cica error');
    }
  }

  addDonation() {
    if (this.amounts.find((x) => {
      return x != 0
    })) {
      let i = 0;
      let req = [];
      for (let item of this.items) {
          req.push({name: item[0], count: this.amounts[i]});
          i++;
      }

      let json = JSON.stringify(req)
      console.log(json);
      const obj = this.data.addDonation(localStorage.getItem('hub'), json);
      obj.subscribe (ans => console.log(ans));
      localStorage.setItem('donation', 'true');
      this.donationErr = false;
      this.router.navigate(['/hubs']);
    } else {
      localStorage.setItem('donation', 'false');
      this.donationErr = true;
    }
  }

}
