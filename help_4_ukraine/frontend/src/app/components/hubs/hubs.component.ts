import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { DataService } from 'src/app/data.service';

@Component({
  selector: 'app-hubs',
  templateUrl: './hubs.component.html',
  styleUrls: ['./hubs.component.css'],
  providers: [DataService]
})
export class HubsComponent implements OnInit {

  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private data: DataService
  ) { }

  hubs = [];

  donationCompleted = false;

  ngOnInit(): void {
    this.data.getHubs().subscribe(items => {
        console.log(items[0]);
        for (let item of items) {
          this.hubs.push(item['name'] + ": " + item['address']);
        }
      })

    if (!localStorage.getItem('token')) {
      this.router.navigate(['/']);
    }
    if (localStorage.getItem('donation') == 'true') {
      this.donationCompleted = true;
    } else {
      this.donationCompleted = false;
    }
  }

  // hubs = ['Hub1', 'Hub2', 'Hub3', 'Hub4', 'Hub5', 'Hub6', 'Hub7', 'Hub8', 'Hub9', 'Hub10']

  hubSelection(index) {
    this.donationCompleted = false;
    localStorage.setItem('donation', 'false');
    localStorage.setItem('hub', index + 1);
    this.router.navigate(['/hub-items']);
  }

}
